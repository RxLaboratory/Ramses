import requests
import json
import uuid
import hashlib

import html2text

token = "8d4b57014169bdc2c3db1ba555b7e1444f25de08"
version = "0.5.0-Rubika_3"
url = "https://ramses.rxlab.io/tests"
clientKey = "drHSV2XQ"

session = requests.Session()

headers = {
    'Accept': 'application/json',
    'Content-Type': 'application/json;charset=utf-8',
    'User-Agent': 'RamServer Dev Tools'
}

def installServer():
    r = session.get(url + "/install/index.php", headers=headers)
    print( html2text.html2text( r.text ) )

def ping():
    data = {
        "version": version
    }
    r = session.post(url + "/?ping", headers=headers, data=json.dumps(data))
    print( html2text.html2text( r.text ) )

def login(username, password):
    global token
    data = {
        "version": version,
        "username": username,
        "password": password
    }
    r = session.post(url + "/dev/?login", headers=headers, data=json.dumps(data))
    token = html2text.html2text( r.text ).strip()
    print(token)

def sync( tables, date ):
    global token
    data = {
        "version": version,
        "token": token,
        "previousSyncDate": date,
        "tables": tables
    }
    r = session.post(url + "/?sync", headers=headers, data=json.dumps(data))
    print( html2text.html2text( r.text ) )

def setPassword( uuid, pswd, current = "" ):
    global token, clientKey, url
    # Client hash
    pswd = url.replace('https://', '').replace('/','') + pswd + clientKey
    pswd = hashlib.sha3_512(pswd.encode()).hexdigest()

    if (current != ""):
        # Client hash
        current = url.replace('https://', '').replace('/','') + current + clientKey
        current = hashlib.sha3_512(current.encode()).hexdigest()
 
    data = {
        "version": version,
        "token": token,
        "uuid": uuid,
        "newPassword": pswd,
        "currentPassword": current
    }
    r = session.post(url + "/?setPassword", headers=headers, data=json.dumps(data))
    print( html2text.html2text( r.text ) )

def setUserName( uuid, username, name ):
    sync( (
        {
            "name": "RamUser",
            "modifiedRows": (
                {
                    "uuid": uuid,
                    "userName": username,
                    "data": '{"name":"' + name + '", "userName":"' + username + '"}',
                    "modified": "2022-08-29 15:00:00",
                    "removed": 0
                },
            )
        },
        ), "1970-01-01 00:00:00"
    )

def clean( tables ):
    global token
    data = {
        "version": version,
        "token": token,
        "tables": tables
    }
    r = session.post(url + "/?clean", headers=headers, data=json.dumps(data))
    print( html2text.html2text( r.text ) )

# TESTS Here

def testSync():
    
    uuid1 = str(uuid.uuid4())
    uuid2 = str(uuid.uuid4())
    uuid3 = str(uuid.uuid4())
    uuid4 = str(uuid.uuid4())
    uuid5 = str(uuid.uuid4())
    uuid6 = str(uuid.uuid4())
    uuid7 = str(uuid.uuid4())

    # 1. 
    print("Create a few items in the tables. Should return an empty list")
    sync( (
        {
            "name": "RamApplication",
            "modifiedRows": (
                {
                    "uuid": uuid1,
                    "data": '{"name":"After Effects", "shortName":"WRONG" }',
                    "modified": "2022-07-15 01:00:00",
                    "removed": 0
                },
                {
                    "uuid":  uuid2,
                    "data": '{"name":"Photoshop", "shortName":"Ps" }',
                    "modified": "2022-07-15 02:00:00",
                    "removed": 0
                }
            )
        },
        {
            "name": "RamFileType",
            "modifiedRows": (
                {
                    "uuid": uuid3,
                    "data": '{"name":"After Effects Project", "shortName":"aep" }',
                    "modified": "2022-07-15 03:00:00",
                    "removed": 0
                },
                {
                    "uuid":  uuid4,
                    "data": '{"name":"Photoshop Document", "shortName":"wrong" }',
                    "modified": "2022-07-15 04:00:00",
                    "removed": 0
                }
            )
        }
    ), "2022-07-15 00:00:00")

    # 2. 
    print("Update a few lines, add new ones. Should return an empty list.")
    sync( (
        {
            "name": "RamApplication",
            "modifiedRows": (
                {
                    "uuid": uuid1,
                    "data": '{"name":"After Effects", "shortName":"Ae" }',
                    "modified": "2022-07-16 05:00:00",
                    "removed": 0
                },
                {
                    "uuid": uuid5,
                    "data": '{"name":"Krita", "shortName":"Krita" }',
                    "modified": "2022-07-16 06:00:00",
                    "removed": 0
                },
            )
        },
        {
            "name": "RamFileType",
            "modifiedRows": (
                {
                    "uuid":  uuid4,
                    "data": '{"name":"Photoshop Document", "shortName":"psd" }',
                    "modified": "2022-07-16 07:00:00",
                    "removed": 0
                },
                {
                    "uuid":  uuid6,
                    "data": '{"name":"Krita Document", "shortName":"kra" }',
                    "modified": "2022-07-16 08:00:00",
                    "removed": 0
                },
            )
        }
    ), "2022-07-16 00:00:00")

    # 3. 
    print("Send an outdated object along with a new one and an updated one. Should return the updated object")
    sync( (
        {
            "name": "RamApplication",
            "modifiedRows": (
                {
                    "uuid": uuid1,
                    "data": '{"name":"After Effects", "shortName":"wrong" }',
                    "modified": "2022-07-16 03:00:00",
                    "removed": 0
                },
                {
                    "uuid": uuid2,
                    "data": '{"name":"Photoshop", "shortName":"Ps", "color":"#232323" }',
                    "modified": "2022-07-16 12:00:00",
                    "removed": 0
                },
                {
                    "uuid": uuid7,
                    "data": '{"name":"Blender", "shortName":"Blender" }',
                    "modified": "2022-07-16 13:00:00",
                    "removed": 0
                },
            )
        },
    ), "2022-07-16 00:00:00")
    
    print("Finished. Check coherence in the DB")

def testSyncUser():
    uuid1 = str(uuid.uuid4())
    uuid2 = str(uuid.uuid4())

    # 1. 
    print("Create an item in the tables. Should return an empty list")
    sync( ({
        "name": "RamUser",
        "modifiedRows": (
            {
                "uuid": uuid1,
                "data": '{"name":"Nicolas Dufresne", "shortName":"Duf" }',
                "modified": "2022-07-15 01:00:00",
                "removed": 0,
                "userName": "Duf"
            },
        )
    },
    ), "2022-07-15 00:00:00")

    # 2. 
    print("Update User. Should return an empty list")
    sync( (
        {
            "name": "RamUser",
            "modifiedRows":
            (
                {
                    "uuid": uuid1,
                    "data": '{"name":"Nicolas Dufresne", "shortName":"Duduf" }',
                    "modified": "2022-07-16 06:00:00",
                    "removed": 0,
                    "userName": "Duduf"
                },
            )
        },
    ), "2022-07-16 00:00:00")

def downloadTables(tableNames):
    tables = []
    for tableName in tableNames:
        table = {
            "name": tableName,
            "modifiedRows": ()
        }
        tables.append(table)
    sync(tables, "1970-01-01 00:00:00")

def testClean():
    clean( ( 
            {
                "name": "RamAsset",
                "rows": [ "9f9abb25-c404-53a1-bd1f-d38ad16a668b", ]
            },
        ))

installServer()

# Always start a session with a ping
ping()
# We need to login before everything else
login("Admin", "password")
# Test empty sync
#sync( (), "2022-07-15 00:00:00")
# Let's test sync
#testSync()
#testSyncUser()
#setUserName( "dda85817-34a4-4a97-a1ae-43e9b04da031", "Duf", "Nicolas Dufresne" )
#login("Admin", "pass")
#setPassword( "cac400e4-dfe1-4005-949e-a085f9aa43bd", "password", "pass" )
#testClean()


"""downloadTables((
    "RamUser",
    ))"""



"""sync( (
        {
            "name": "RamApplication",
            "modifiedRows": (
                {
                    "uuid": "b1287769-7645-4702-b593-000a2d1e3771",
                    "data": '{"name":"After Effects", "shortName":"WRONG" }',
                    "modified": "2022-07-16 00:00:00",
                    "removed": 0
                },
                {
                    "uuid":  "92316258-67f5-4f18-a8a6-46d284577f66",
                    "data": '{"name":"Photoshop", "shortName":"Ps" }',
                    "modified": "2022-07-16 00:00:00",
                    "removed": 0
                }
            )
        },
    ), "2022-07-15 00:00:00")"""