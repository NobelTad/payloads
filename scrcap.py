import subprocess

def run_capture_cpp():
    result = subprocess.run(["build/scrcap.exe"], stdout=subprocess.PIPE, text=True)
    path = result.stdout.strip()
    return path

a = run_capture_cpp()
print("Screenshot saved at:", a)
