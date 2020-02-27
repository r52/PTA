let item = {
    "item": {
        "armour": {
            "es": 224,
            "enabled": false
        },
        "category": "armour.chest",
        "filters": {
            "crafted.stat_3143208761": {
                "id": "crafted.stat_3143208761",
                "text": "#% increased Attributes",
                "type": "crafted",
                "value": [6],
                "enabled": false
            },
            "explicit.stat_1220361974": {
                "id": "explicit.stat_1220361974",
                "text": "Killed Enemies Explode, dealing #% of their Life as Physical Damage",
                "type": "explicit",
                "value": [3],
                "enabled": false
            },
            "explicit.stat_30642521": {
                "id": "explicit.stat_30642521",
                "text": "You can apply an additional Curse",
                "type": "explicit",
                "value": [],
                "enabled": false
            },
            "explicit.stat_3372524247": {
                "id": "explicit.stat_3372524247",
                "text": "#% to Fire Resistance",
                "type": "explicit",
                "value": [38],
                "enabled": false
            },
            "explicit.stat_3594640492": {
                "id": "explicit.stat_3594640492",
                "text": "Regenerate #% of Energy Shield per second",
                "type": "explicit",
                "value": [1],
                "enabled": false
            },
            "explicit.stat_4052037485": {
                "id": "explicit.stat_4052037485",
                "text": "# to maximum Energy Shield (Local)",
                "type": "explicit",
                "value": [9],
                "enabled": false
            }
        },
        "ilvl": 100,
        "influences": ["crusader", "hunter"],
        "name": "Morbid Guardian",
        "origtext": "Rarity: Rare\nMorbid Guardian\nVaal Regalia\n--------\nQuality: +30% (augmented)\nEnergy Shield: 224 (augmented)\n--------\nRequirements:\nLevel: 68\nStr: 105\nDex: 96\nInt: 194\n--------\nSockets: W-W-W-W-B-W \n--------\nItem Level: 100\n--------\n+9 to maximum Energy Shield\n+38% to Fire Resistance\nYou can apply an additional Curse\nRegenerate 1% of Energy Shield per second\nKilled Enemies Explode, dealing 3% of their Life as Physical Damage\n6% increased Attributes (crafted)\n--------\nCrusader Item\nHunter Item\n",
        "pseudos": {
            "pseudo.pseudo_total_elemental_resistance": {
                "id": "pseudo.pseudo_total_elemental_resistance",
                "text": "+#% total Elemental Resistance",
                "type": "pseudo",
                "value": [38],
                "enabled": false
            },
            "pseudo.pseudo_total_fire_resistance": {
                "id": "pseudo.pseudo_total_fire_resistance",
                "text": "+#% total to Fire Resistance",
                "type": "pseudo",
                "value": [38],
                "enabled": false
            }
        },
        "quality": 30,
        "rarity": "Rare",
        "requirements": {
            "dex": 96,
            "int": 194,
            "lvl": 68,
            "str": 105
        },
        "sockets": {
            "A": 0,
            "B": 1,
            "G": 0,
            "R": 0,
            "W": 5,
            "links": 6,
            "total": 6
        },
        "type": "Vaal Regalia"
    },
    "settings": {
        "buyoutonly": true,
        "corrupt_override": false,
        "corrupt_search": "Any",
        "display_limit": 12,
        "league": "Metamorph",
        "onlineonly": true,
        "prefillbase": true,
        "prefillilvl": false,
        "prefillmax": false,
        "prefillmin": true,
        "prefillnormals": false,
        "prefillpseudos": false,
        "primary_currency": "chaos",
        "removedupes": true,
        "secondary_currency": "exa"
    },
    "searchopts": {
        "use_pdps": {
            enabled: false
        },
        "use_edps": {
            enabled: false
        },
        "use_ar": {
            enabled: false
        },
        "use_ev": {
            enabled: false
        },
        "use_es": {
            enabled: false
        },
        "use_sockets": false,
        "use_links": false,
        "use_ilvl": false,
        "use_item_base": true,
        "influences": ["crusader", "hunter"]
    }
}

module.exports.item = item;
