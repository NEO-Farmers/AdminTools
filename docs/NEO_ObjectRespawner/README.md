# NEO Object Respawner
## Description
This mod was originally meant to respawn targets at a shooting range, but has been adjusted
to be more generic. 

The vanilla object spawner has a field for items labeled "customString" add the following to it 
to get certain behaviors. The mod will lowercase the string and only checks for the presence of these substrings
so they may appear anywhere in that field and have the listed effect.
"neorespawn" - will cause the item to respawn upon delete 
"neoreporthit" - will send a notification to the player of hits on targets 
"neoreporttake" - will send a notification to the player not to abuse take-able respawning objects and logs it to the script log.

example 
```
{
    "Objects": [
        {
            "name": "LargeGasCanister",
            "pos": [
                3110.197998046875,
                372.1195373535156,
                6792.9072265625
            ],
            "ypr": [
                0.0,
                0.0,
                -0.0
            ],
            "scale": 1.0,
            "enableCEPersistency": 0,
            "customString": "neorespawn neoreporthit neoreporttake"
        },
        {
            "name": "ZmbM_JournalistSkinny",
            "pos": [
                3448.330078125,
                345.64532470703127,
                6428.83837890625
            ],
            "ypr": [
                20.59589385986328,
                0.45636898279190066,
                -0.5629247426986694
            ],
            "scale": 0.9999732971191406,
            "enableCEPersistency": 0,
            "customString": "neorespawn neoreporthit"
        }
    ]
}
```