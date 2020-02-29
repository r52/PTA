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
        "prefillrange": 0,
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
        "influences": ["crusader", "hunter"],
        "use_corrupted": "Any"
    },
    "results": {
        "format": "advanced",
        "options": "Metamorph, Use Base Type, Crusader Influence, Hunter Influence, Corrupted=Any",
        "result": [{
            "id": "c45cfc3653af43d2484cbe52b10dc3eb32cfd3f017f33164d95378b71207cece",
            "item": {
                "craftedMods": ["+84 to maximum Life"],
                "explicitMods": ["+32% to Fire Resistance", "Regenerate 1% of Life per second", "You can apply an additional Curse", "Regenerate 0.6% of Energy Shield per second", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 212,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_3299347043", [0]]
                        ],
                        "explicit": [
                            ["explicit.stat_3372524247", [3]],
                            ["explicit.stat_836936635", [4]],
                            ["explicit.stat_30642521", [0]],
                            ["explicit.stat_3594640492", [2]],
                            ["explicit.stat_1220361974", [1]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_3299347043",
                                "max": 85,
                                "min": 71
                            }],
                            "name": "Upgraded",
                            "tier": "R5"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3594640492",
                                "max": 0.6,
                                "min": 0.6
                            }],
                            "name": "of Vibrance",
                            "tier": "S2"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3372524247",
                                "max": 35,
                                "min": 30
                            }],
                            "name": "of the Furnace",
                            "tier": "S4"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_836936635",
                                "max": 1.5,
                                "min": 1
                            }],
                            "name": "of the Hunt",
                            "tier": "S2"
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpWaWN0b3J5IFdyYXANClZhYWwgUmVnYWxpYQ0KLS0tLS0tLS0NClF1YWxpdHk6ICszMCUgKGF1Z21lbnRlZCkNCkVuZXJneSBTaGllbGQ6IDIxMiAoYXVnbWVudGVkKQ0KLS0tLS0tLS0NClJlcXVpcmVtZW50czoNCkxldmVsOiA2OA0KSW50OiAxOTQNCi0tLS0tLS0tDQpTb2NrZXRzOiBXLVctVy1XLUItVyANCi0tLS0tLS0tDQpJdGVtIExldmVsOiAxMDANCi0tLS0tLS0tDQorMzIlIHRvIEZpcmUgUmVzaXN0YW5jZQ0KUmVnZW5lcmF0ZSAxJSBvZiBMaWZlIHBlciBzZWNvbmQNCllvdSBjYW4gYXBwbHkgYW4gYWRkaXRpb25hbCBDdXJzZQ0KUmVnZW5lcmF0ZSAwLjYlIG9mIEVuZXJneSBTaGllbGQgcGVyIHNlY29uZA0KS2lsbGVkIEVuZW1pZXMgRXhwbG9kZSwgZGVhbGluZyAzJSBvZiB0aGVpciBMaWZlIGFzIFBoeXNpY2FsIERhbWFnZQ0KKzg0IHRvIG1heGltdW0gTGlmZSAoY3JhZnRlZCkNCi0tLS0tLS0tDQpDcnVzYWRlciBJdGVtDQpIdW50ZXIgSXRlbQ0KLS0tLS0tLS0NCk5vdGU6IH5wcmljZSA3IGV4YQ0K"
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Victory Wrap",
                "note": "~price 7 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+30%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["212", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "CoC_POW",
                    "name": "misterchen",
                    "online": {
                        "league": "Metamorph"
                    }
                },
                "indexed": "2020-02-26T04:52:09Z",
                "method": "psapi",
                "price": {
                    "amount": 7,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "Sell",
                    "x": 7,
                    "y": 6
                },
                "whisper": "@CoC_POW Hi, I would like to buy your Victory Wrap Vaal Regalia listed for 7 exa in Metamorph (stash tab \"Sell\"; position: left 8, top 7)"
            }
        }, {
            "id": "3db389d88077aa6fcfca79daa17f1664054a175a83153d4d6bdaae653294b1d5",
            "item": {
                "craftedMods": ["+32% to Fire Resistance"],
                "explicitMods": ["+13 to maximum Energy Shield", "+17 to maximum Mana", "You can apply an additional Curse", "Regenerate 0.6% of Energy Shield per second", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 222,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_3372524247", [0]]
                        ],
                        "explicit": [
                            ["explicit.stat_4052037485", [3]],
                            ["explicit.stat_1050105434", [3]],
                            ["explicit.stat_30642521", [0]],
                            ["explicit.stat_3594640492", [2]],
                            ["explicit.stat_1220361974", [1]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_3372524247",
                                "max": 35,
                                "min": 29
                            }],
                            "name": "of Craft",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3594640492",
                                "max": 0.6,
                                "min": 0.6
                            }],
                            "name": "of Vibrance",
                            "tier": "S2"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_4052037485",
                                "max": 15,
                                "min": 11
                            }, {
                                "hash": "explicit.stat_1050105434",
                                "max": 19,
                                "min": 16
                            }],
                            "name": "Deacon's",
                            "tier": "P3"
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpNaW5kIFNoZWxsDQpWYWFsIFJlZ2FsaWENCi0tLS0tLS0tDQpRdWFsaXR5OiArMjYlIChhdWdtZW50ZWQpDQpFbmVyZ3kgU2hpZWxkOiAyMjIgKGF1Z21lbnRlZCkNCi0tLS0tLS0tDQpSZXF1aXJlbWVudHM6DQpMZXZlbDogNjgNCkludDogMTk0DQotLS0tLS0tLQ0KU29ja2V0czogRy1HLUctQi1HLUIgDQotLS0tLS0tLQ0KSXRlbSBMZXZlbDogMTAwDQotLS0tLS0tLQ0KKzEzIHRvIG1heGltdW0gRW5lcmd5IFNoaWVsZA0KKzE3IHRvIG1heGltdW0gTWFuYQ0KWW91IGNhbiBhcHBseSBhbiBhZGRpdGlvbmFsIEN1cnNlDQpSZWdlbmVyYXRlIDAuNiUgb2YgRW5lcmd5IFNoaWVsZCBwZXIgc2Vjb25kDQpLaWxsZWQgRW5lbWllcyBFeHBsb2RlLCBkZWFsaW5nIDMlIG9mIHRoZWlyIExpZmUgYXMgUGh5c2ljYWwgRGFtYWdlDQorMzIlIHRvIEZpcmUgUmVzaXN0YW5jZSAoY3JhZnRlZCkNCi0tLS0tLS0tDQpDcnVzYWRlciBJdGVtDQpIdW50ZXIgSXRlbQ0KLS0tLS0tLS0NCk5vdGU6IH5wcmljZSAxMCBleGENCg=="
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Mind Shell",
                "note": "~price 10 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+26%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["222", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "Mynameisjeeeeefffffffff",
                    "name": "OMFGdarkside",
                    "online": {
                        "league": "Metamorph",
                        "status": "afk"
                    }
                },
                "indexed": "2020-02-27T01:34:50Z",
                "method": "psapi",
                "price": {
                    "amount": 10,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "Blight",
                    "x": 4,
                    "y": 8
                },
                "whisper": "@Mynameisjeeeeefffffffff Hi, I would like to buy your Mind Shell Vaal Regalia listed for 10 exa in Metamorph (stash tab \"Blight\"; position: left 5, top 9)"
            }
        }, {
            "id": "b8d10a29807df6758d9ed5d71e007ca2d00f88950f17805be79c88ac7c919a09",
            "item": {
                "explicitMods": ["+52 to Intelligence", "+63 to maximum Energy Shield", "Regenerate 17.2 Life per second", "Regenerate 1.5% of Life per second", "You can apply an additional Curse", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 292,
                    "hashes": {
                        "explicit": [
                            ["explicit.stat_328541901", [3]],
                            ["explicit.stat_4052037485", [5]],
                            ["explicit.stat_3325883026", [2]],
                            ["explicit.stat_836936635", [4]],
                            ["explicit.stat_30642521", [1]],
                            ["explicit.stat_1220361974", [0]]
                        ]
                    },
                    "mods": {
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3325883026",
                                "max": 20,
                                "min": 16
                            }],
                            "name": "of Ryslatha",
                            "tier": "S2"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_328541901",
                                "max": 55,
                                "min": 51
                            }],
                            "name": "of the Genius",
                            "tier": "S1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_836936635",
                                "max": 1.5,
                                "min": 1
                            }],
                            "name": "of the Hunt",
                            "tier": "S2"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_4052037485",
                                "max": 76,
                                "min": 62
                            }],
                            "name": "Scintillating",
                            "tier": "P3"
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpFbXB5cmVhbiBHdWFyZGlhbg0KVmFhbCBSZWdhbGlhDQotLS0tLS0tLQ0KUXVhbGl0eTogKzI5JSAoYXVnbWVudGVkKQ0KRW5lcmd5IFNoaWVsZDogMjkyIChhdWdtZW50ZWQpDQotLS0tLS0tLQ0KUmVxdWlyZW1lbnRzOg0KTGV2ZWw6IDY4DQpJbnQ6IDE5NA0KLS0tLS0tLS0NClNvY2tldHM6IEctRy1HLUctQi1CIA0KLS0tLS0tLS0NCkl0ZW0gTGV2ZWw6IDg1DQotLS0tLS0tLQ0KKzUyIHRvIEludGVsbGlnZW5jZQ0KKzYzIHRvIG1heGltdW0gRW5lcmd5IFNoaWVsZA0KUmVnZW5lcmF0ZSAxNy4yIExpZmUgcGVyIHNlY29uZA0KUmVnZW5lcmF0ZSAxLjUlIG9mIExpZmUgcGVyIHNlY29uZA0KWW91IGNhbiBhcHBseSBhbiBhZGRpdGlvbmFsIEN1cnNlDQpLaWxsZWQgRW5lbWllcyBFeHBsb2RlLCBkZWFsaW5nIDMlIG9mIHRoZWlyIExpZmUgYXMgUGh5c2ljYWwgRGFtYWdlDQotLS0tLS0tLQ0KQ3J1c2FkZXIgSXRlbQ0KSHVudGVyIEl0ZW0NCi0tLS0tLS0tDQpOb3RlOiB+Yi9vIDEwIGV4YQ0K"
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 85,
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Empyrean Guardian",
                "note": "~b/o 10 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+29%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["292", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "oviore",
                    "name": "DanyGagnon1",
                    "online": {
                        "league": "Metamorph"
                    }
                },
                "indexed": "2020-02-27T17:59:48Z",
                "method": "psapi",
                "price": {
                    "amount": 10,
                    "currency": "exa",
                    "type": "~b/o"
                },
                "stash": {
                    "name": "DUMP",
                    "x": 5,
                    "y": 4
                },
                "whisper": "@oviore Hi, I would like to buy your Empyrean Guardian Vaal Regalia listed for 10 exa in Metamorph (stash tab \"DUMP\"; position: left 6, top 5)"
            }
        }, {
            "id": "3e79fad12a74f0a7ac4c9286c6d26c76dc63ed080fdbc2e09c3bd5b320694374",
            "item": {
                "craftedMods": ["+20% to Fire and Lightning Resistances"],
                "explicitMods": ["+30 to Intelligence", "+20 to maximum Energy Shield", "+20 to maximum Mana", "You can apply an additional Curse", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 238,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_3441501978", [0]]
                        ],
                        "explicit": [
                            ["explicit.stat_328541901", [3]],
                            ["explicit.stat_4052037485", [2]],
                            ["explicit.stat_1050105434", [2]],
                            ["explicit.stat_30642521", [0]],
                            ["explicit.stat_1220361974", [1]]
                        ],
                        "implicit": [
                            ["implicit.stat_3513534186", [0]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_3441501978",
                                "max": 20,
                                "min": 17
                            }],
                            "name": "of Craft",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_4052037485",
                                "max": 25,
                                "min": 16
                            }, {
                                "hash": "explicit.stat_1050105434",
                                "max": 22,
                                "min": 20
                            }],
                            "name": "Priest's ",
                            "tier": "P2"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_328541901",
                                "max": 32,
                                "min": 28
                            }],
                            "name": "of the Philosopher",
                            "tier": "S5"
                        }],
                        "implicit": [{
                            "magnitudes": [{
                                "hash": "implicit.stat_3513534186",
                                "max": 2,
                                "min": 2
                            }],
                            "name": "",
                            "tier": ""
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpEcmVhZCBXYXJkDQpWYWFsIFJlZ2FsaWENCi0tLS0tLS0tDQpRdWFsaXR5OiArMzAlIChhdWdtZW50ZWQpDQpFbmVyZ3kgU2hpZWxkOiAyMzggKGF1Z21lbnRlZCkNCi0tLS0tLS0tDQpSZXF1aXJlbWVudHM6DQpMZXZlbDogNjgNCkludDogMTk0DQotLS0tLS0tLQ0KU29ja2V0czogVy1XLVctVy1CLVcgDQotLS0tLS0tLQ0KSXRlbSBMZXZlbDogMTAwDQotLS0tLS0tLQ0KSXRlbSBzZWxscyBmb3IgbXVjaCBtb3JlIHRvIHZlbmRvcnMgKGltcGxpY2l0KQ0KLS0tLS0tLS0NCiszMCB0byBJbnRlbGxpZ2VuY2UNCisyMCB0byBtYXhpbXVtIEVuZXJneSBTaGllbGQNCisyMCB0byBtYXhpbXVtIE1hbmENCllvdSBjYW4gYXBwbHkgYW4gYWRkaXRpb25hbCBDdXJzZQ0KS2lsbGVkIEVuZW1pZXMgRXhwbG9kZSwgZGVhbGluZyAzJSBvZiB0aGVpciBMaWZlIGFzIFBoeXNpY2FsIERhbWFnZQ0KKzIwJSB0byBGaXJlIGFuZCBMaWdodG5pbmcgUmVzaXN0YW5jZXMgKGNyYWZ0ZWQpDQotLS0tLS0tLQ0KQ3J1c2FkZXIgSXRlbQ0KSHVudGVyIEl0ZW0NCi0tLS0tLS0tDQpOb3RlOiB+cHJpY2UgMTIgZXhhDQo="
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "implicitMods": ["Item sells for much more to vendors"],
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Dread Ward",
                "note": "~price 12 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+30%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["238", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "StayStiizy",
                    "name": "iMnMs",
                    "online": {
                        "league": "Metamorph"
                    }
                },
                "indexed": "2020-02-27T07:24:00Z",
                "method": "psapi",
                "price": {
                    "amount": 12,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "shop 2",
                    "x": 8,
                    "y": 6
                },
                "whisper": "@StayStiizy Hi, I would like to buy your Dread Ward Vaal Regalia listed for 12 exa in Metamorph (stash tab \"shop 2\"; position: left 9, top 7)"
            }
        }, {
            "id": "32a92d35b4e72b22c0eb922a9435a14b5a8e742d9c0dc900fbada05f1873fc3c",
            "item": {
                "craftedMods": ["Can have up to 3 Crafted Modifiers", "6% increased Attributes", "Gain 10% of Maximum Life as Extra Maximum Energy Shield"],
                "explicitMods": ["You can apply an additional Curse", "11% increased Energy Shield Recovery rate", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 212,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_1859333175", [0]],
                            ["crafted.stat_3143208761", [2]],
                            ["crafted.stat_67280387", [1]]
                        ],
                        "explicit": [
                            ["explicit.stat_30642521", [1]],
                            ["explicit.stat_988575597", [2]],
                            ["explicit.stat_1220361974", [0]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_1859333175",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "of Crafting",
                            "tier": "S"
                        }, {
                            "magnitudes": [{
                                "hash": "crafted.stat_67280387",
                                "max": 10,
                                "min": 9
                            }],
                            "name": "Gravicius'",
                            "tier": "R3"
                        }, {
                            "magnitudes": [{
                                "hash": "crafted.stat_3143208761",
                                "max": 6,
                                "min": 6
                            }],
                            "name": "of Hillock",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_988575597",
                                "max": 11,
                                "min": 8
                            }],
                            "name": "of the Crusade",
                            "tier": "S2"
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpTb2wgQ2xvYWsNClZhYWwgUmVnYWxpYQ0KLS0tLS0tLS0NClF1YWxpdHk6ICszMCUgKGF1Z21lbnRlZCkNCkVuZXJneSBTaGllbGQ6IDIxMiAoYXVnbWVudGVkKQ0KLS0tLS0tLS0NClJlcXVpcmVtZW50czoNCkxldmVsOiA2OA0KSW50OiAxOTQNCi0tLS0tLS0tDQpTb2NrZXRzOiBXLVctVy1XLUItVyANCi0tLS0tLS0tDQpJdGVtIExldmVsOiAxMDANCi0tLS0tLS0tDQpZb3UgY2FuIGFwcGx5IGFuIGFkZGl0aW9uYWwgQ3Vyc2UNCjExJSBpbmNyZWFzZWQgRW5lcmd5IFNoaWVsZCBSZWNvdmVyeSByYXRlDQpLaWxsZWQgRW5lbWllcyBFeHBsb2RlLCBkZWFsaW5nIDMlIG9mIHRoZWlyIExpZmUgYXMgUGh5c2ljYWwgRGFtYWdlDQpDYW4gaGF2ZSB1cCB0byAzIENyYWZ0ZWQgTW9kaWZpZXJzIChjcmFmdGVkKQ0KNiUgaW5jcmVhc2VkIEF0dHJpYnV0ZXMgKGNyYWZ0ZWQpDQpHYWluIDEwJSBvZiBNYXhpbXVtIExpZmUgYXMgRXh0cmEgTWF4aW11bSBFbmVyZ3kgU2hpZWxkIChjcmFmdGVkKQ0KLS0tLS0tLS0NCkNydXNhZGVyIEl0ZW0NCkh1bnRlciBJdGVtDQotLS0tLS0tLQ0KTm90ZTogfnByaWNlIDIwIGV4YQ0K"
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Sol Cloak",
                "note": "~price 20 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+30%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["212", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "Ayaka",
                    "name": "DragonAyakaDanteDiamond",
                    "online": {
                        "league": "Standard",
                        "status": "afk"
                    }
                },
                "indexed": "2020-02-13T02:10:30Z",
                "method": "psapi",
                "price": {
                    "amount": 20,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "S1",
                    "x": 6,
                    "y": 0
                },
                "whisper": "@Ayaka Hi, I would like to buy your Sol Cloak Vaal Regalia listed for 20 exa in Metamorph (stash tab \"S1\"; position: left 7, top 1)"
            }
        }, {
            "id": "617a91cbe9547c466f6c692dab67e2e557c8d30e5741b9343cc262c0d44debaa",
            "item": {
                "craftedMods": ["Can have up to 3 Crafted Modifiers", "+19% to Cold and Lightning Resistances", "Gain 10% of Maximum Life as Extra Maximum Energy Shield"],
                "explicitMods": ["+22% to Lightning Resistance", "You can apply an additional Curse", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 196,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_1859333175", [0]],
                            ["crafted.stat_4277795662", [2]],
                            ["crafted.stat_67280387", [1]]
                        ],
                        "explicit": [
                            ["explicit.stat_1671376347", [2]],
                            ["explicit.stat_30642521", [1]],
                            ["explicit.stat_1220361974", [0]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_1859333175",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "of Crafting",
                            "tier": "S"
                        }, {
                            "magnitudes": [{
                                "hash": "crafted.stat_67280387",
                                "max": 10,
                                "min": 9
                            }],
                            "name": "Gravicius'",
                            "tier": "R3"
                        }, {
                            "magnitudes": [{
                                "hash": "crafted.stat_4277795662",
                                "max": 20,
                                "min": 17
                            }],
                            "name": "of Craft",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_1671376347",
                                "max": 23,
                                "min": 18
                            }],
                            "name": "of the Storm",
                            "tier": "S6"
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpCbG9vZCBIaWRlDQpWYWFsIFJlZ2FsaWENCi0tLS0tLS0tDQpRdWFsaXR5OiArMjAlIChhdWdtZW50ZWQpDQpFbmVyZ3kgU2hpZWxkOiAxOTYgKGF1Z21lbnRlZCkNCi0tLS0tLS0tDQpSZXF1aXJlbWVudHM6DQpMZXZlbDogNjgNCkludDogMTk0DQotLS0tLS0tLQ0KU29ja2V0czogRy1HLVItQi1SLUIgDQotLS0tLS0tLQ0KSXRlbSBMZXZlbDogODQNCi0tLS0tLS0tDQorMjIlIHRvIExpZ2h0bmluZyBSZXNpc3RhbmNlDQpZb3UgY2FuIGFwcGx5IGFuIGFkZGl0aW9uYWwgQ3Vyc2UNCktpbGxlZCBFbmVtaWVzIEV4cGxvZGUsIGRlYWxpbmcgMyUgb2YgdGhlaXIgTGlmZSBhcyBQaHlzaWNhbCBEYW1hZ2UNCkNhbiBoYXZlIHVwIHRvIDMgQ3JhZnRlZCBNb2RpZmllcnMgKGNyYWZ0ZWQpDQorMTklIHRvIENvbGQgYW5kIExpZ2h0bmluZyBSZXNpc3RhbmNlcyAoY3JhZnRlZCkNCkdhaW4gMTAlIG9mIE1heGltdW0gTGlmZSBhcyBFeHRyYSBNYXhpbXVtIEVuZXJneSBTaGllbGQgKGNyYWZ0ZWQpDQotLS0tLS0tLQ0KQ3J1c2FkZXIgSXRlbQ0KSHVudGVyIEl0ZW0NCi0tLS0tLS0tDQpOb3RlOiB+cHJpY2UgMjAgZXhhDQo="
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 84,
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Blood Hide",
                "note": "~price 20 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+20%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["196", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "S",
                    "group": 0,
                    "sColour": "R"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "S",
                    "group": 0,
                    "sColour": "R"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "uwumybussy",
                    "name": "beeballer94",
                    "online": {
                        "league": "Metamorph"
                    }
                },
                "indexed": "2020-01-24T05:26:40Z",
                "method": "psapi",
                "price": {
                    "amount": 20,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "s",
                    "x": 2,
                    "y": 9
                },
                "whisper": "@uwumybussy Hi, I would like to buy your Blood Hide Vaal Regalia listed for 20 exa in Metamorph (stash tab \"s\"; position: left 3, top 10)"
            }
        }, {
            "id": "d312bc77b6f127ddf6294a7e8cb3b2e30cfb146ab3d43627c5b2f0feaedc05d9",
            "item": {
                "craftedMods": ["+15% to Fire and Chaos Resistances"],
                "explicitMods": ["+46% to Fire Resistance", "Regenerate 1.18% of Life per second", "You can apply an additional Curse", "11% of Physical Damage from Hits taken as Chaos Damage", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 212,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_378817135", [0]]
                        ],
                        "explicit": [
                            ["explicit.stat_3372524247", [4]],
                            ["explicit.stat_836936635", [3]],
                            ["explicit.stat_30642521", [0]],
                            ["explicit.stat_4129825612", [2]],
                            ["explicit.stat_1220361974", [1]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_378817135",
                                "max": 15,
                                "min": 13
                            }],
                            "name": "of the Order",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_4129825612",
                                "max": 15,
                                "min": 11
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_836936635",
                                "max": 1.5,
                                "min": 1
                            }],
                            "name": "of the Hunt",
                            "tier": "S2"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3372524247",
                                "max": 48,
                                "min": 46
                            }],
                            "name": "of Tzteosh",
                            "tier": "S1"
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpUZW1wZXN0IEd1YXJkaWFuDQpWYWFsIFJlZ2FsaWENCi0tLS0tLS0tDQpRdWFsaXR5OiArMzAlIChhdWdtZW50ZWQpDQpFbmVyZ3kgU2hpZWxkOiAyMTIgKGF1Z21lbnRlZCkNCi0tLS0tLS0tDQpSZXF1aXJlbWVudHM6DQpMZXZlbDogNjgNCkludDogMTk0DQotLS0tLS0tLQ0KU29ja2V0czogVy1XLVctVy1CLVcgDQotLS0tLS0tLQ0KSXRlbSBMZXZlbDogMTAwDQotLS0tLS0tLQ0KKzQ2JSB0byBGaXJlIFJlc2lzdGFuY2UNClJlZ2VuZXJhdGUgMS4xOCUgb2YgTGlmZSBwZXIgc2Vjb25kDQpZb3UgY2FuIGFwcGx5IGFuIGFkZGl0aW9uYWwgQ3Vyc2UNCjExJSBvZiBQaHlzaWNhbCBEYW1hZ2UgZnJvbSBIaXRzIHRha2VuIGFzIENoYW9zIERhbWFnZQ0KS2lsbGVkIEVuZW1pZXMgRXhwbG9kZSwgZGVhbGluZyAzJSBvZiB0aGVpciBMaWZlIGFzIFBoeXNpY2FsIERhbWFnZQ0KKzE1JSB0byBGaXJlIGFuZCBDaGFvcyBSZXNpc3RhbmNlcyAoY3JhZnRlZCkNCi0tLS0tLS0tDQpDcnVzYWRlciBJdGVtDQpIdW50ZXIgSXRlbQ0KLS0tLS0tLS0NCk5vdGU6IH5wcmljZSAyMiBleGENCg=="
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Tempest Guardian",
                "note": "~price 22 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+30%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["212", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "ko_KR",
                    "lastCharacterName": "투명엑잘",
                    "name": "ychakill",
                    "online": {
                        "league": "Metamorph",
                        "status": "afk"
                    }
                },
                "indexed": "2020-02-27T05:15:01Z",
                "method": "psapi",
                "price": {
                    "amount": 22,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "장사3",
                    "x": 6,
                    "y": 5
                },
                "whisper": "@투명엑잘 안녕하세요, 변형(보관함 탭 \"장사3\", 위치: 왼쪽 7, 상단 6)에 22 exa(으)로 올려놓은 폭풍 가디언 바알 예복(을)를 구매하고 싶습니다"
            }
        }, {
            "id": "b1b0fbbe6c34be0029fa0a936155a23a849a7fdd2050ea829bc459046b6d8f9f",
            "item": {
                "craftedMods": ["+35% to Fire Resistance"],
                "explicitMods": ["+35% to Cold Resistance", "24% increased Stun and Block Recovery", "You can apply an additional Curse", "Reflects 27 Physical Damage to Melee Attackers", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 212,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_3372524247", [0]]
                        ],
                        "explicit": [
                            ["explicit.stat_4220027924", [3]],
                            ["explicit.stat_2511217560", [4]],
                            ["explicit.stat_30642521", [1]],
                            ["explicit.stat_3767873853", [2]],
                            ["explicit.stat_1220361974", [0]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_3372524247",
                                "max": 35,
                                "min": 29
                            }],
                            "name": "of Craft",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3767873853",
                                "max": 50,
                                "min": 25
                            }],
                            "name": "Jagged",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_4220027924",
                                "max": 35,
                                "min": 30
                            }],
                            "name": "of the Walrus",
                            "tier": "S4"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_2511217560",
                                "max": 25,
                                "min": 23
                            }],
                            "name": "of Adamantite Skin",
                            "tier": "S2"
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpSdW5lIFNhbHZhdGlvbg0KVmFhbCBSZWdhbGlhDQotLS0tLS0tLQ0KUXVhbGl0eTogKzMwJSAoYXVnbWVudGVkKQ0KRW5lcmd5IFNoaWVsZDogMjEyIChhdWdtZW50ZWQpDQotLS0tLS0tLQ0KUmVxdWlyZW1lbnRzOg0KTGV2ZWw6IDY4DQpJbnQ6IDE5NA0KLS0tLS0tLS0NClNvY2tldHM6IFctVy1XLVctQi1XIA0KLS0tLS0tLS0NCkl0ZW0gTGV2ZWw6IDEwMA0KLS0tLS0tLS0NCiszNSUgdG8gQ29sZCBSZXNpc3RhbmNlDQoyNCUgaW5jcmVhc2VkIFN0dW4gYW5kIEJsb2NrIFJlY292ZXJ5DQpZb3UgY2FuIGFwcGx5IGFuIGFkZGl0aW9uYWwgQ3Vyc2UNClJlZmxlY3RzIDI3IFBoeXNpY2FsIERhbWFnZSB0byBNZWxlZSBBdHRhY2tlcnMNCktpbGxlZCBFbmVtaWVzIEV4cGxvZGUsIGRlYWxpbmcgMyUgb2YgdGhlaXIgTGlmZSBhcyBQaHlzaWNhbCBEYW1hZ2UNCiszNSUgdG8gRmlyZSBSZXNpc3RhbmNlIChjcmFmdGVkKQ0KLS0tLS0tLS0NCkNydXNhZGVyIEl0ZW0NCkh1bnRlciBJdGVtDQotLS0tLS0tLQ0KTm90ZTogfnByaWNlIDIzIGV4YQ0K"
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Rune Salvation",
                "note": "~price 23 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+30%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["212", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "BigDickBoomer",
                    "name": "JediWabbit",
                    "online": {
                        "league": "Metamorph"
                    }
                },
                "indexed": "2020-02-27T23:53:05Z",
                "method": "psapi",
                "price": {
                    "amount": 23,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "OFFERS = NO",
                    "x": 10,
                    "y": 6
                },
                "whisper": "@BigDickBoomer Hi, I would like to buy your Rune Salvation Vaal Regalia listed for 23 exa in Metamorph (stash tab \"OFFERS = NO\"; position: left 11, top 7)"
            }
        }, {
            "id": "0fdb3dd9e207cea3c5bab666222a588201ee627fa8ce5400fd3dd78f46f87332",
            "item": {
                "corrupted": true,
                "craftedMods": ["+22 to Strength", "+18% to Quality"],
                "explicitMods": ["+33 to Intelligence", "+88 to maximum Energy Shield", "Recover 5% of Energy Shield on Kill", "You can apply an additional Curse", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 371,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_4080418644", [0]],
                            ["crafted.stat_2016708976", [0]]
                        ],
                        "explicit": [
                            ["explicit.stat_328541901", [3]],
                            ["explicit.stat_4052037485", [2]],
                            ["explicit.stat_2406605753", [4]],
                            ["explicit.stat_30642521", [1]],
                            ["explicit.stat_1220361974", [0]]
                        ],
                        "implicit": [
                            ["implicit.stat_3513534186", [0]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_4080418644",
                                "max": 25,
                                "min": 21
                            }, {
                                "hash": "crafted.stat_2016708976",
                                "max": 18,
                                "min": 15
                            }],
                            "name": "of the Order",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_4052037485",
                                "max": 90,
                                "min": 77
                            }],
                            "name": "Incandescent",
                            "tier": "P2"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_328541901",
                                "max": 37,
                                "min": 33
                            }],
                            "name": "of the Sage",
                            "tier": "S4"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_2406605753",
                                "max": 6,
                                "min": 5
                            }],
                            "name": "of the Crusade",
                            "tier": "S1"
                        }],
                        "implicit": [{
                            "magnitudes": [{
                                "hash": "implicit.stat_3513534186",
                                "max": 2,
                                "min": 2
                            }],
                            "name": "",
                            "tier": ""
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpUZW1wZXN0IFNoZWxsDQpWYWFsIFJlZ2FsaWENCi0tLS0tLS0tDQpRdWFsaXR5OiArNDglIChhdWdtZW50ZWQpDQpFbmVyZ3kgU2hpZWxkOiAzNzEgKGF1Z21lbnRlZCkNCi0tLS0tLS0tDQpSZXF1aXJlbWVudHM6DQpMZXZlbDogNjgNCkludDogMTk0DQotLS0tLS0tLQ0KU29ja2V0czogVy1XLVctVy1CLVcgDQotLS0tLS0tLQ0KSXRlbSBMZXZlbDogMTAwDQotLS0tLS0tLQ0KSXRlbSBzZWxscyBmb3IgbXVjaCBtb3JlIHRvIHZlbmRvcnMgKGltcGxpY2l0KQ0KLS0tLS0tLS0NCiszMyB0byBJbnRlbGxpZ2VuY2UNCis4OCB0byBtYXhpbXVtIEVuZXJneSBTaGllbGQNClJlY292ZXIgNSUgb2YgRW5lcmd5IFNoaWVsZCBvbiBLaWxsDQpZb3UgY2FuIGFwcGx5IGFuIGFkZGl0aW9uYWwgQ3Vyc2UNCktpbGxlZCBFbmVtaWVzIEV4cGxvZGUsIGRlYWxpbmcgMyUgb2YgdGhlaXIgTGlmZSBhcyBQaHlzaWNhbCBEYW1hZ2UNCisyMiB0byBTdHJlbmd0aCAoY3JhZnRlZCkNCisxOCUgdG8gUXVhbGl0eSAoY3JhZnRlZCkNCi0tLS0tLS0tDQpDb3JydXB0ZWQNCi0tLS0tLS0tDQpDcnVzYWRlciBJdGVtDQpIdW50ZXIgSXRlbQ0KLS0tLS0tLS0NCk5vdGU6IH5iL28gMjMuOSBleGENCg=="
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "implicitMods": ["Item sells for much more to vendors"],
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Tempest Shell",
                "note": "~b/o 23.9 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+48%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["371", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "ColonCleanserr",
                    "name": "cathartist",
                    "online": {
                        "league": "Metamorph",
                        "status": "afk"
                    }
                },
                "indexed": "2020-02-24T13:48:01Z",
                "method": "psapi",
                "price": {
                    "amount": 23.9,
                    "currency": "exa",
                    "type": "~b/o"
                },
                "stash": {
                    "name": "F",
                    "x": 2,
                    "y": 5
                },
                "whisper": "@ColonCleanserr Hi, I would like to buy your Tempest Shell Vaal Regalia listed for 23.9 exa in Metamorph (stash tab \"F\"; position: left 3, top 6)"
            }
        }, {
            "id": "5ecfdd22b8a9abad4a70632be96534cd541ebbe78fcce9420d31f42aa83452ee",
            "item": {
                "craftedMods": ["+32% to Fire Resistance"],
                "explicitMods": ["18% reduced Attribute Requirements", "+51 to Intelligence", "+90 to maximum Energy Shield", "You can apply an additional Curse", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 329,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_3372524247", [0]]
                        ],
                        "explicit": [
                            ["explicit.stat_3639275092", [4]],
                            ["explicit.stat_328541901", [2]],
                            ["explicit.stat_4052037485", [3]],
                            ["explicit.stat_30642521", [0]],
                            ["explicit.stat_1220361974", [1]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_3372524247",
                                "max": 35,
                                "min": 29
                            }],
                            "name": "of Craft",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_328541901",
                                "max": 55,
                                "min": 51
                            }],
                            "name": "of the Genius",
                            "tier": "S1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_4052037485",
                                "max": 90,
                                "min": 77
                            }],
                            "name": "Incandescent",
                            "tier": "P2"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3639275092",
                                "max": -18,
                                "min": -18
                            }],
                            "name": "of the Worthy",
                            "tier": "S2"
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpEYW1uYXRpb24gQ2FyYXBhY2UNClZhYWwgUmVnYWxpYQ0KLS0tLS0tLS0NClF1YWxpdHk6ICszMCUgKGF1Z21lbnRlZCkNCkVuZXJneSBTaGllbGQ6IDMyOSAoYXVnbWVudGVkKQ0KLS0tLS0tLS0NClJlcXVpcmVtZW50czoNCkxldmVsOiA2OA0KSW50OiAxNTkgKGF1Z21lbnRlZCkNCi0tLS0tLS0tDQpTb2NrZXRzOiBXLVctVy1XLUItVyANCi0tLS0tLS0tDQpJdGVtIExldmVsOiAxMDANCi0tLS0tLS0tDQoxOCUgcmVkdWNlZCBBdHRyaWJ1dGUgUmVxdWlyZW1lbnRzDQorNTEgdG8gSW50ZWxsaWdlbmNlDQorOTAgdG8gbWF4aW11bSBFbmVyZ3kgU2hpZWxkDQpZb3UgY2FuIGFwcGx5IGFuIGFkZGl0aW9uYWwgQ3Vyc2UNCktpbGxlZCBFbmVtaWVzIEV4cGxvZGUsIGRlYWxpbmcgMyUgb2YgdGhlaXIgTGlmZSBhcyBQaHlzaWNhbCBEYW1hZ2UNCiszMiUgdG8gRmlyZSBSZXNpc3RhbmNlIChjcmFmdGVkKQ0KLS0tLS0tLS0NCkNydXNhZGVyIEl0ZW0NCkh1bnRlciBJdGVtDQotLS0tLS0tLQ0KTm90ZTogfnByaWNlIDI1IGV4YQ0K"
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Damnation Carapace",
                "note": "~price 25 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+30%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["329", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["159", 1]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "heyitsJoey",
                    "name": "GirlfriendRiven",
                    "online": {
                        "league": "Metamorph"
                    }
                },
                "indexed": "2020-02-28T00:01:55Z",
                "method": "psapi",
                "price": {
                    "amount": 25,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "s2",
                    "x": 2,
                    "y": 4
                },
                "whisper": "@heyitsJoey Hi, I would like to buy your Damnation Carapace Vaal Regalia listed for 25 exa in Metamorph (stash tab \"s2\"; position: left 3, top 5)"
            }
        }, {
            "id": "caf5df372ab0a1037281699bb88a0202f06936f30da8f0dbd12e79fb3b5eef63",
            "item": {
                "craftedMods": ["Can have up to 3 Crafted Modifiers", "+33% to Fire Resistance", "+19% to Fire and Cold Resistances"],
                "explicitMods": ["+86 to maximum Life", "You can apply an additional Curse", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 210,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_1859333175", [0]],
                            ["crafted.stat_3372524247", [1]],
                            ["crafted.stat_2915988346", [2]]
                        ],
                        "explicit": [
                            ["explicit.stat_3299347043", [2]],
                            ["explicit.stat_30642521", [1]],
                            ["explicit.stat_1220361974", [0]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_1859333175",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "of Crafting",
                            "tier": "S"
                        }, {
                            "magnitudes": [{
                                "hash": "crafted.stat_3372524247",
                                "max": 35,
                                "min": 29
                            }],
                            "name": "of Craft",
                            "tier": "R3"
                        }, {
                            "magnitudes": [{
                                "hash": "crafted.stat_2915988346",
                                "max": 20,
                                "min": 17
                            }],
                            "name": "of Craft",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3299347043",
                                "max": 89,
                                "min": 80
                            }],
                            "name": "Athlete's",
                            "tier": "P5"
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpTcGlyaXQgSGlkZQ0KVmFhbCBSZWdhbGlhDQotLS0tLS0tLQ0KUXVhbGl0eTogKzI5JSAoYXVnbWVudGVkKQ0KRW5lcmd5IFNoaWVsZDogMjEwIChhdWdtZW50ZWQpDQotLS0tLS0tLQ0KUmVxdWlyZW1lbnRzOg0KTGV2ZWw6IDY4DQpJbnQ6IDE5NA0KLS0tLS0tLS0NClNvY2tldHM6IFItUi1HLUItUi1HIA0KLS0tLS0tLS0NCkl0ZW0gTGV2ZWw6IDg1DQotLS0tLS0tLQ0KKzg2IHRvIG1heGltdW0gTGlmZQ0KWW91IGNhbiBhcHBseSBhbiBhZGRpdGlvbmFsIEN1cnNlDQpLaWxsZWQgRW5lbWllcyBFeHBsb2RlLCBkZWFsaW5nIDMlIG9mIHRoZWlyIExpZmUgYXMgUGh5c2ljYWwgRGFtYWdlDQpDYW4gaGF2ZSB1cCB0byAzIENyYWZ0ZWQgTW9kaWZpZXJzIChjcmFmdGVkKQ0KKzMzJSB0byBGaXJlIFJlc2lzdGFuY2UgKGNyYWZ0ZWQpDQorMTklIHRvIEZpcmUgYW5kIENvbGQgUmVzaXN0YW5jZXMgKGNyYWZ0ZWQpDQotLS0tLS0tLQ0KQ3J1c2FkZXIgSXRlbQ0KSHVudGVyIEl0ZW0NCi0tLS0tLS0tDQpOb3RlOiB+cHJpY2UgMjUgZXhhDQo="
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 85,
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Spirit Hide",
                "note": "~price 25 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+29%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["210", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "S",
                    "group": 0,
                    "sColour": "R"
                }, {
                    "attr": "S",
                    "group": 0,
                    "sColour": "R"
                }, {
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "S",
                    "group": 0,
                    "sColour": "R"
                }, {
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "BLUEㆍCC",
                    "name": "bluecc",
                    "online": {
                        "league": "Metamorph"
                    }
                },
                "indexed": "2020-02-27T03:47:50Z",
                "method": "psapi",
                "price": {
                    "amount": 25,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "S",
                    "x": 1,
                    "y": 3
                },
                "whisper": "@BLUEㆍCC Hi, I would like to buy your Spirit Hide Vaal Regalia listed for 25 exa in Metamorph (stash tab \"S\"; position: left 2, top 4)"
            }
        }, {
            "id": "5b0852fed635a9a6f32f0c5587b35ac8bb0eb67b8db17a01f6b9cc638be39fd6",
            "item": {
                "craftedMods": ["Can have up to 3 Crafted Modifiers", "+19% to Fire and Lightning Resistances", "Gain 10% of Maximum Life as Extra Maximum Energy Shield"],
                "explicitMods": ["+41% to Cold Resistance", "You can apply an additional Curse", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 212,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_1859333175", [0]],
                            ["crafted.stat_3441501978", [2]],
                            ["crafted.stat_67280387", [1]]
                        ],
                        "explicit": [
                            ["explicit.stat_4220027924", [2]],
                            ["explicit.stat_30642521", [1]],
                            ["explicit.stat_1220361974", [0]]
                        ],
                        "implicit": [
                            ["implicit.stat_3513534186", [0]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_1859333175",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "of Crafting",
                            "tier": "S"
                        }, {
                            "magnitudes": [{
                                "hash": "crafted.stat_67280387",
                                "max": 10,
                                "min": 9
                            }],
                            "name": "Gravicius'",
                            "tier": "R3"
                        }, {
                            "magnitudes": [{
                                "hash": "crafted.stat_3441501978",
                                "max": 20,
                                "min": 17
                            }],
                            "name": "of Craft",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_4220027924",
                                "max": 41,
                                "min": 36
                            }],
                            "name": "of the Polar Bear",
                            "tier": "S3"
                        }],
                        "implicit": [{
                            "magnitudes": [{
                                "hash": "implicit.stat_3513534186",
                                "max": 2,
                                "min": 2
                            }],
                            "name": "",
                            "tier": ""
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpLcmFrZW4gU2hlbGwNClZhYWwgUmVnYWxpYQ0KLS0tLS0tLS0NClF1YWxpdHk6ICszMCUgKGF1Z21lbnRlZCkNCkVuZXJneSBTaGllbGQ6IDIxMiAoYXVnbWVudGVkKQ0KLS0tLS0tLS0NClJlcXVpcmVtZW50czoNCkxldmVsOiA2OA0KSW50OiAxOTQNCi0tLS0tLS0tDQpTb2NrZXRzOiBXLVctVy1XLUItVyANCi0tLS0tLS0tDQpJdGVtIExldmVsOiAxMDANCi0tLS0tLS0tDQpJdGVtIHNlbGxzIGZvciBtdWNoIG1vcmUgdG8gdmVuZG9ycyAoaW1wbGljaXQpDQotLS0tLS0tLQ0KKzQxJSB0byBDb2xkIFJlc2lzdGFuY2UNCllvdSBjYW4gYXBwbHkgYW4gYWRkaXRpb25hbCBDdXJzZQ0KS2lsbGVkIEVuZW1pZXMgRXhwbG9kZSwgZGVhbGluZyAzJSBvZiB0aGVpciBMaWZlIGFzIFBoeXNpY2FsIERhbWFnZQ0KQ2FuIGhhdmUgdXAgdG8gMyBDcmFmdGVkIE1vZGlmaWVycyAoY3JhZnRlZCkNCisxOSUgdG8gRmlyZSBhbmQgTGlnaHRuaW5nIFJlc2lzdGFuY2VzIChjcmFmdGVkKQ0KR2FpbiAxMCUgb2YgTWF4aW11bSBMaWZlIGFzIEV4dHJhIE1heGltdW0gRW5lcmd5IFNoaWVsZCAoY3JhZnRlZCkNCi0tLS0tLS0tDQpDcnVzYWRlciBJdGVtDQpIdW50ZXIgSXRlbQ0KLS0tLS0tLS0NCk5vdGU6IH5wcmljZSAzMCBleGENCg=="
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "implicitMods": ["Item sells for much more to vendors"],
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Kraken Shell",
                "note": "~price 30 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+30%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["212", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "Jonxx______",
                    "name": "Jonxxpl",
                    "online": {
                        "league": "Metamorph"
                    }
                },
                "indexed": "2020-02-22T11:32:47Z",
                "method": "psapi",
                "price": {
                    "amount": 30,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "$",
                    "x": 16,
                    "y": 18
                },
                "whisper": "@Jonxx______ Hi, I would like to buy your Kraken Shell Vaal Regalia listed for 30 exa in Metamorph (stash tab \"$\"; position: left 17, top 19)"
            }
        }, {
            "id": "7c3e8d9bcc097b8f6b16cd92ab2f9c6a4a64215f87961d23fa12216f4fd4b350",
            "item": {
                "craftedMods": ["+85 to maximum Life"],
                "explicitMods": ["+11% to Fire Resistance", "+34% to Lightning Resistance", "You can apply an additional Curse", "14% increased Energy Shield Recovery rate", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 212,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_3299347043", [0]]
                        ],
                        "explicit": [
                            ["explicit.stat_3372524247", [2]],
                            ["explicit.stat_1671376347", [4]],
                            ["explicit.stat_30642521", [1]],
                            ["explicit.stat_988575597", [3]],
                            ["explicit.stat_1220361974", [0]]
                        ],
                        "implicit": [
                            ["implicit.stat_3513534186", [0]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_3299347043",
                                "max": 85,
                                "min": 71
                            }],
                            "name": "Upgraded",
                            "tier": "R5"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3372524247",
                                "max": 11,
                                "min": 6
                            }],
                            "name": "of the Whelpling",
                            "tier": "S8"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_988575597",
                                "max": 15,
                                "min": 12
                            }],
                            "name": "of the Crusade",
                            "tier": "S1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_1671376347",
                                "max": 35,
                                "min": 30
                            }],
                            "name": "of the Tempest",
                            "tier": "S4"
                        }],
                        "implicit": [{
                            "magnitudes": [{
                                "hash": "implicit.stat_3513534186",
                                "max": 2,
                                "min": 2
                            }],
                            "name": "",
                            "tier": ""
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpCbGlnaHQgQ29hdA0KVmFhbCBSZWdhbGlhDQotLS0tLS0tLQ0KUXVhbGl0eTogKzMwJSAoYXVnbWVudGVkKQ0KRW5lcmd5IFNoaWVsZDogMjEyIChhdWdtZW50ZWQpDQotLS0tLS0tLQ0KUmVxdWlyZW1lbnRzOg0KTGV2ZWw6IDY4DQpJbnQ6IDE5NA0KLS0tLS0tLS0NClNvY2tldHM6IFctQi1CLUctQi1CIA0KLS0tLS0tLS0NCkl0ZW0gTGV2ZWw6IDEwMA0KLS0tLS0tLS0NCkl0ZW0gc2VsbHMgZm9yIG11Y2ggbW9yZSB0byB2ZW5kb3JzIChpbXBsaWNpdCkNCi0tLS0tLS0tDQorMTElIHRvIEZpcmUgUmVzaXN0YW5jZQ0KKzM0JSB0byBMaWdodG5pbmcgUmVzaXN0YW5jZQ0KWW91IGNhbiBhcHBseSBhbiBhZGRpdGlvbmFsIEN1cnNlDQoxNCUgaW5jcmVhc2VkIEVuZXJneSBTaGllbGQgUmVjb3ZlcnkgcmF0ZQ0KS2lsbGVkIEVuZW1pZXMgRXhwbG9kZSwgZGVhbGluZyAzJSBvZiB0aGVpciBMaWZlIGFzIFBoeXNpY2FsIERhbWFnZQ0KKzg1IHRvIG1heGltdW0gTGlmZSAoY3JhZnRlZCkNCi0tLS0tLS0tDQpDcnVzYWRlciBJdGVtDQpIdW50ZXIgSXRlbQ0KLS0tLS0tLS0NCk5vdGU6IH5wcmljZSA1MCBleGENCg=="
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "implicitMods": ["Item sells for much more to vendors"],
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Blight Coat",
                "note": "~price 50 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+30%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["212", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "D",
                    "group": 0,
                    "sColour": "G"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "Bow_is_Meta",
                    "name": "Tabert",
                    "online": {
                        "league": "Metamorph"
                    }
                },
                "indexed": "2020-02-24T01:16:52Z",
                "method": "psapi",
                "price": {
                    "amount": 50,
                    "currency": "exa",
                    "type": "~price"
                },
                "stash": {
                    "name": "Meta$$",
                    "x": 10,
                    "y": 0
                },
                "whisper": "@Bow_is_Meta Hi, I would like to buy your Blight Coat Vaal Regalia listed for 50 exa in Metamorph (stash tab \"Meta$$\"; position: left 11, top 1)"
            }
        }, {
            "id": "7d2275e7f90a015abb9f9b5794cff24a83ebc5fc64832ae266fe4b17847061e2",
            "item": {
                "craftedMods": ["+17% to Fire and Lightning Resistances"],
                "explicitMods": ["+110 to maximum Life", "+21% to Lightning Resistance", "You can apply an additional Curse", "Attacks have +1.5% to Critical Strike Chance", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 212,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_3441501978", [0]]
                        ],
                        "explicit": [
                            ["explicit.stat_3299347043", [2]],
                            ["explicit.stat_1671376347", [3]],
                            ["explicit.stat_30642521", [0]],
                            ["explicit.stat_2572042788", [4]],
                            ["explicit.stat_1220361974", [1]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_3441501978",
                                "max": 20,
                                "min": 17
                            }],
                            "name": "of Craft",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3299347043",
                                "max": 119,
                                "min": 110
                            }],
                            "name": "Rapturous",
                            "tier": "P2"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_1671376347",
                                "max": 23,
                                "min": 18
                            }],
                            "name": "of the Storm",
                            "tier": "S6"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_2572042788",
                                "max": 1.5,
                                "min": 1.1
                            }],
                            "name": "of the Hunt",
                            "tier": "S1"
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpDb3Jwc2UgV2FyZA0KVmFhbCBSZWdhbGlhDQotLS0tLS0tLQ0KUXVhbGl0eTogKzMwJSAoYXVnbWVudGVkKQ0KRW5lcmd5IFNoaWVsZDogMjEyIChhdWdtZW50ZWQpDQotLS0tLS0tLQ0KUmVxdWlyZW1lbnRzOg0KTGV2ZWw6IDY4DQpJbnQ6IDE5NA0KLS0tLS0tLS0NClNvY2tldHM6IFctVy1XLVctQi1XIA0KLS0tLS0tLS0NCkl0ZW0gTGV2ZWw6IDEwMA0KLS0tLS0tLS0NCisxMTAgdG8gbWF4aW11bSBMaWZlDQorMjElIHRvIExpZ2h0bmluZyBSZXNpc3RhbmNlDQpZb3UgY2FuIGFwcGx5IGFuIGFkZGl0aW9uYWwgQ3Vyc2UNCkF0dGFja3MgaGF2ZSArMS41JSB0byBDcml0aWNhbCBTdHJpa2UgQ2hhbmNlDQpLaWxsZWQgRW5lbWllcyBFeHBsb2RlLCBkZWFsaW5nIDMlIG9mIHRoZWlyIExpZmUgYXMgUGh5c2ljYWwgRGFtYWdlDQorMTclIHRvIEZpcmUgYW5kIExpZ2h0bmluZyBSZXNpc3RhbmNlcyAoY3JhZnRlZCkNCi0tLS0tLS0tDQpDcnVzYWRlciBJdGVtDQpIdW50ZXIgSXRlbQ0KLS0tLS0tLS0NCk5vdGU6IH5iL28gMjAwIGV4YQ0K"
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Corpse Ward",
                "note": "~b/o 200 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+30%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["212", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "DD_DienDien",
                    "name": "bestfriendyb",
                    "online": {
                        "league": "Metamorph",
                        "status": "afk"
                    }
                },
                "indexed": "2020-02-12T11:56:20Z",
                "method": "psapi",
                "price": {
                    "amount": 200,
                    "currency": "exa",
                    "type": "~b/o"
                },
                "stash": {
                    "name": "sell",
                    "x": 3,
                    "y": 0
                },
                "whisper": "@DD_DienDien Hi, I would like to buy your Corpse Ward Vaal Regalia listed for 200 exa in Metamorph (stash tab \"sell\"; position: left 4, top 1)"
            }
        }, {
            "id": "35a7e95b51d876d180bba17078562e6463f7e67110b628d32f231b5b369ea7d0",
            "item": {
                "craftedMods": ["+30 to Intelligence"],
                "explicitMods": ["+107 to maximum Life", "+32% to Fire Resistance", "You can apply an additional Curse", "Attacks have +1.5% to Critical Strike Chance", "Killed Enemies Explode, dealing 3% of their Life as Physical Damage"],
                "extended": {
                    "es": 212,
                    "hashes": {
                        "crafted": [
                            ["crafted.stat_328541901", [0]]
                        ],
                        "explicit": [
                            ["explicit.stat_3299347043", [2]],
                            ["explicit.stat_3372524247", [4]],
                            ["explicit.stat_30642521", [0]],
                            ["explicit.stat_2572042788", [3]],
                            ["explicit.stat_1220361974", [1]]
                        ],
                        "implicit": [
                            ["implicit.stat_3513534186", [0]]
                        ]
                    },
                    "mods": {
                        "crafted": [{
                            "magnitudes": [{
                                "hash": "crafted.stat_328541901",
                                "max": 30,
                                "min": 26
                            }],
                            "name": "of Craft",
                            "tier": "R3"
                        }],
                        "explicit": [{
                            "magnitudes": [{
                                "hash": "explicit.stat_30642521",
                                "max": 1,
                                "min": 1
                            }],
                            "name": "Hunter's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_1220361974",
                                "max": 3,
                                "min": 3
                            }],
                            "name": "Crusader's",
                            "tier": "P1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3299347043",
                                "max": 109,
                                "min": 100
                            }],
                            "name": "Vigorous",
                            "tier": "P3"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_2572042788",
                                "max": 1.5,
                                "min": 1.1
                            }],
                            "name": "of the Hunt",
                            "tier": "S1"
                        }, {
                            "magnitudes": [{
                                "hash": "explicit.stat_3372524247",
                                "max": 35,
                                "min": 30
                            }],
                            "name": "of the Furnace",
                            "tier": "S4"
                        }],
                        "implicit": [{
                            "magnitudes": [{
                                "hash": "implicit.stat_3513534186",
                                "max": 2,
                                "min": 2
                            }],
                            "name": "",
                            "tier": ""
                        }]
                    },
                    "text": "UmFyaXR5OiBSYXJlDQpCZWhlbW90aCBNYW50bGUNClZhYWwgUmVnYWxpYQ0KLS0tLS0tLS0NClF1YWxpdHk6ICszMCUgKGF1Z21lbnRlZCkNCkVuZXJneSBTaGllbGQ6IDIxMiAoYXVnbWVudGVkKQ0KLS0tLS0tLS0NClJlcXVpcmVtZW50czoNCkxldmVsOiA2OA0KSW50OiAxOTQNCi0tLS0tLS0tDQpTb2NrZXRzOiBXLVctVy1XLUItVyANCi0tLS0tLS0tDQpJdGVtIExldmVsOiAxMDANCi0tLS0tLS0tDQpJdGVtIHNlbGxzIGZvciBtdWNoIG1vcmUgdG8gdmVuZG9ycyAoaW1wbGljaXQpDQotLS0tLS0tLQ0KKzEwNyB0byBtYXhpbXVtIExpZmUNCiszMiUgdG8gRmlyZSBSZXNpc3RhbmNlDQpZb3UgY2FuIGFwcGx5IGFuIGFkZGl0aW9uYWwgQ3Vyc2UNCkF0dGFja3MgaGF2ZSArMS41JSB0byBDcml0aWNhbCBTdHJpa2UgQ2hhbmNlDQpLaWxsZWQgRW5lbWllcyBFeHBsb2RlLCBkZWFsaW5nIDMlIG9mIHRoZWlyIExpZmUgYXMgUGh5c2ljYWwgRGFtYWdlDQorMzAgdG8gSW50ZWxsaWdlbmNlIChjcmFmdGVkKQ0KLS0tLS0tLS0NCkNydXNhZGVyIEl0ZW0NCkh1bnRlciBJdGVtDQotLS0tLS0tLQ0KTm90ZTogfmIvbyAyNTAgZXhhDQo="
                },
                "frameType": 2,
                "h": 3,
                "icon": "https://web.poecdn.com/image/Art/2DItems/Armours/BodyArmours/BodyStrDexInt1C.png?scale=1&w=2&h=3&v=5a4eb9d863bef835aa3d9cc9224f51a5",
                "identified": true,
                "ilvl": 100,
                "implicitMods": ["Item sells for much more to vendors"],
                "influences": {
                    "crusader": true,
                    "hunter": true
                },
                "league": "Metamorph",
                "name": "Behemoth Mantle",
                "note": "~b/o 250 exa",
                "properties": [{
                    "displayMode": 0,
                    "name": "Quality",
                    "type": 6,
                    "values": [
                        ["+30%", 1]
                    ]
                }, {
                    "displayMode": 0,
                    "name": "Energy Shield",
                    "type": 18,
                    "values": [
                        ["212", 1]
                    ]
                }],
                "requirements": [{
                    "displayMode": 0,
                    "name": "Level",
                    "values": [
                        ["68", 0]
                    ]
                }, {
                    "displayMode": 1,
                    "name": "Int",
                    "values": [
                        ["194", 0]
                    ]
                }],
                "sockets": [{
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }, {
                    "attr": "I",
                    "group": 0,
                    "sColour": "B"
                }, {
                    "attr": "G",
                    "group": 0,
                    "sColour": "W"
                }],
                "typeLine": "Vaal Regalia",
                "verified": true,
                "w": 2
            },
            "listing": {
                "account": {
                    "language": "en_US",
                    "lastCharacterName": "Gumbo_yaya",
                    "name": "skol_gumbo",
                    "online": {
                        "league": "Metamorph"
                    }
                },
                "indexed": "2020-02-28T02:33:59Z",
                "method": "psapi",
                "price": {
                    "amount": 250,
                    "currency": "exa",
                    "type": "~b/o"
                },
                "stash": {
                    "name": "Prem2",
                    "x": 4,
                    "y": 8
                },
                "whisper": "@Gumbo_yaya Hi, I would like to buy your Behemoth Mantle Vaal Regalia listed for 250 exa in Metamorph (stash tab \"Prem2\"; position: left 5, top 9)"
            }
        }]
    }
}

module.exports.item = item;
