# OpenHAB integration

_Work in progress_

## Configuration

Although the ZauberBoxOP software follows Homie convention I use a Generic MQTT configuration for now.

Note about the naming:

_First use case is to control a espresso machine and the adjacent kitchen lights, and also the thermostat of the (floor) heating in the kitchen.
Therefore the device is called "CoffeeCtrl", but has "Nodes" for heating and lights._

### Things/Channels

```
Thing mqtt:topic:coffeectrl "Kaffee Controller" (mqtt:broker:IAmq) @ "Küche" {
	Type string:	coffee_HomieState	"CoffeeCtrl State"			[stateTopic="homie/CoffeeCtrl/$state", allowedStates= "init,ready,disconnected,sleeping,lost,alert" ]
	Type string:	coffee_white_cmd	"CoffeeCtrl Command White"	[stateTopic="homie/CoffeeCtrl/RGBWCtrl/w_cmd" ]
	Type string:	coffee_heatTemp_cmd	"CoffeeCtrl Command Heater"	[stateTopic="homie/CoffeeCtrl/heatSet/setTempCmd", postCommand="true" ] 
	Type number:	coffee_heatTemp_val	"CoffeeCtrl Value Heater"	[stateTopic="homie/CoffeeCtrl/heatSet/setTemp", commandTopic="homie/CoffeeCtrl/heatSet/setTemp/set" ]
	Type number:	coffee_heatTemp_act	"CoffeeCtrl Actual  Heater"	[stateTopic="homie/CoffeeCtrl/heatSet/actTemp", commandTopic="homie/CoffeeCtrl/heatSet/actTemp/set" ]
	Type switch:	coffee_coffeeMain	"CoffeeCtrl Main State"		[stateTopic="homie/CoffeeCtrl/coffeetimer/coffeetimer_mainstate", commandTopic="homie/CoffeeCtrl/coffeetimer/coffeetimer_mainstate/set", postCommand="true" ]
}
```

### Items

```
Group	CoffeeCtrl												(Homie_Device, Kueche)
String	CoffeeHomieState		"Coffee ctrl state" 			(CoffeeCtrl, Homie_State)	{channel="mqtt:topic:coffeectrl:coffee_HomieState"}
String	CoffeeWhiteCmd			"Coffee ctrl command white"		(CoffeeCtrl)				{channel="mqtt:topic:coffeectrl:coffee_white_cmd"}
String	CoffeeHeatTempCmd		"Coffee ctrl command heater"	(CoffeeCtrl)				{channel="mqtt:topic:coffeectrl:coffee_heatTemp_cmd"}
Switch	CoffeeCoffeeMain		"Coffee ctrl main state" 		(CoffeeCtrl)				{channel="mqtt:topic:coffeectrl:coffee_coffeeMain"}
```

To show the actual room temperature for thermostat menu, the channel "follows" the real channel.
The set-Temperature was an "internal" item before (not connected to any binding, only used in rules).

```
Number	Kueche_temp				"Temperatur [%.1f °C]"			(kueche_thled, gTemperature)	{ channel="mqtt:topic:kueche_thled:Sensor_degrees", channel="mqtt:topic:coffeectrl:coffee_heatTemp_act"[profile="system:follow"] }
Number  Kueche_solltemp		"Soll-Temperatur Küche [%.2f °C]"	<heating>		(Kueche, gTsoll)		["HVAC"]	{channel="mqtt:topic:coffeectrl:coffee_heatTemp_val"}
```


### Rules (jython with "Helper-Libraries" https://openhab-scripters.github.io/openhab-helper-libraries/index.html)

```python
from core.rules import rule
from core.triggers import when

@rule("receive command white from OP Box Kueche")
@when("Item CoffeeWhiteCmd changed")
def kuOpBoxWhite(event):
    cmd = event.itemState.toString()
    if cmd == "NoOp": return
    #targetItem = "thermo_ian_LED_w"
    targetItem = "kueche_thled_LED_w"
    kuOpBoxWhite.log.debug(u"OpBox Command {}".format(event))
    events.postUpdate("CoffeeWhiteCmd", "ACK")     # command ACK
    if cmd == "TOGGLE":
        cmd = "ON" if items[targetItem].as(OnOffType) == OFF else "OFF"
    kuOpBoxWhite.log.debug(u"Target {} // {}".format(items[targetItem].as(OnOffType), items[targetItem].as(PercentType)))
    kuOpBoxWhite.log.debug(u"Sendig command {} to {}".format(cmd, targetItem))

    events.sendCommand(targetItem, cmd)

    
@rule("receive command heater from OP Box Kueche")
@when("Item CoffeeHeatTempCmd received command")
def kuOpBoxHeat(event):    
    kuOpBoxHeat.log.debug(event)
    cmd = event.itemCommand.toString()
    targetItem = "Kueche_solltemp"
    setValue = items[targetItem].as(DecimalType).floatValue()
    if (cmd == "INCREMENT"):
        setValue += 0.2
    elif (cmd == "DECREMENT"):        
        setValue -= 0.2
    elif (cmd == "READ"):
        kuOpBoxHeat.log.info("READ cmd")
    else:
        kuOpBoxHeat.log.warn("Unknown cmd "+cmd)
        return
    
    events.sendCommand(targetItem, str(setValue))


@rule("receive command mainstate from OP Box Kueche")
@when("Item CoffeeCoffeeMain received command")
def kuOpBoxCoffeeMain(event):    
    kuOpBoxCoffeeMain.log.debug(event)
    cmd = event.itemCommand.toString()
    state = items["plug_coffee_State"].as(OnOffType)
    if (str(state) == cmd):
        kuOpBoxCoffeeMain.log.debug("State already " + str(state) + ".")
    else:
        events.sendCommand("plug_coffee_State", cmd)
        

```


