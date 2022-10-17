import subprocess

print("Killing running instance")
subprocess.run((
    "taskkill",
    "/f",
    "/im",
    "maya.exe"
    ))
print("Launching new instance")
subprocess.Popen((
    "C:\\Program Files\\Autodesk\\Maya2023\\bin\\maya.exe",
    "-noAutoloadPlugins"
    ), stdin=None, stdout=None, stderr=None, close_fds=True)
print("Done!")
