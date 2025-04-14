import subprocess
import os

# Start in current dir
cwd = os.getcwd()

while True:
    try:
        cmd = input(f"{cwd}> ").strip()
        if not cmd:
            continue

        # Handle directory change manually
        if cmd.lower().startswith("cd "):
            path = cmd[3:].strip().strip('"')
            new_path = os.path.abspath(os.path.join(cwd, path))
            if os.path.isdir(new_path):
                cwd = new_path
            else:
                print(f"[-] Directory not found: {path}")
            continue

        # Run command silently
        result = subprocess.run(cmd, shell=True, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        output = result.stdout.strip()
        error = result.stderr.strip()

        if output:
            print(output)
        if error:
            print(error)

    except KeyboardInterrupt:
        print("\n[!] Exiting...")
        break
    except Exception as e:
        print(f"[!] Error: {e}")
