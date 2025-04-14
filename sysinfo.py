import wmi
import psutil
import platform
import socket
import requests  # For fetching public IP
import socket
def get_os_info():
    print("[+] OS Version:", platform.system(), platform.release())

def get_cpu_info(w):
    print("\n==== CPU INFO ====")
    for cpu in w.Win32_Processor():
        print("[+] Name:", cpu.Name)
        print("    Manufacturer:", cpu.Manufacturer)
        print("    Max Clock Speed:", cpu.MaxClockSpeed)
        print("    Number Of Cores:", cpu.NumberOfCores)
        print("    Logical Processors:", cpu.NumberOfLogicalProcessors)
        print("    Architecture:", cpu.Architecture)
        print("    Caption:", cpu.Caption)
        print("    Family:", cpu.Family)
        print("    Processor ID:", cpu.ProcessorId)
        print("    Socket:", cpu.SocketDesignation)
        print()

def get_ram_info():
    print("\n[+] Total RAM:", round(psutil.virtual_memory().total / (1024 ** 2)), "MB")

def get_disk_info(w):
    print("\n==== DISK INFO ====")
    for disk in w.Win32_LogicalDisk():
        if disk.Size is not None:
            total_size_gb = round(int(disk.Size) / (1024 ** 3))
        else:
            total_size_gb = "N/A"
        
        if disk.FreeSpace is not None:
            free_space_gb = round(int(disk.FreeSpace) / (1024 ** 3))
        else:
            free_space_gb = "N/A"

        print("[+] Drive Letter:", disk.DeviceID)
        print("    File System:", disk.FileSystem)
        print("    Total Size:", total_size_gb, "GB")
        print("    Free Space:", free_space_gb, "GB")
        print("    Volume Name:", disk.VolumeName)
        print("    Volume Serial Number:", disk.VolumeSerialNumber)
        print("    Media Type:", disk.MediaType)
        print()


def get_gpu_info(w):
    print("\n==== GPU INFO ====")
    for gpu in w.Win32_VideoController():
        print("[+] Name:", gpu.Name)
        print("    Adapter Compatibility:", gpu.AdapterCompatibility)
        print("    Video Processor:", gpu.VideoProcessor)
        print("    Adapter RAM:", int(gpu.AdapterRAM) // (1024 ** 2), "MB")
        print("    Driver Version:", gpu.DriverVersion)
        print()

def get_network_info():
    print("\n==== NETWORK INFO ====")
    # Get the local network interfaces
    for iface, addrs in psutil.net_if_addrs().items():
        print("[+] Adapter:", iface)
        for addr in addrs:
            if addr.family == socket.AF_INET:  # Use socket.AF_INET for IPv4 addresses
                print("    IPv4 Address:", addr.address)
            elif addr.family == psutil.AF_LINK:
                print("    MAC Address:", addr.address)

    # Get public IP address using an external service (e.g., `https://api.ipify.org`)
    try:
        public_ip = socket.gethostbyname("api.ipify.org")
        print("[+] Public IP Address:", public_ip)
    except socket.error:
        print("[!] Unable to retrieve public IP.")

    # Fetching gateway info
    gateways = psutil.net_if_stats()
    for iface, gateway in gateways.items():
        print(f"    {iface} Gateway: {gateway}")
    
    # Fetch public IP
    try:
        public_ip = requests.get('https://api.ipify.org').text
        print("[+] Public IP:", public_ip)
    except requests.exceptions.RequestException as e:
        print("[!] Could not fetch public IP:", e)

def get_motherboard_info(w):
    print("\n==== MOTHERBOARD INFO ====")
    for board in w.Win32_BaseBoard():
        print("[+] Manufacturer:", board.Manufacturer)
        print("    Product:", board.Product)
        print("    Serial Number:", board.SerialNumber)
        print("    Version:", board.Version)
        print()

def get_bios_info(w):
    print("\n==== BIOS INFO ====")
    for bios in w.Win32_BIOS():
        print("[+] Manufacturer:", bios.Manufacturer)
        print("    Version:", bios.Version)
        print("    Serial Number:", bios.SerialNumber)
        print("    Release Date:", bios.ReleaseDate)
        print()

def get_system_info(w):
    print("\n==== SYSTEM INFO ====")
    for sys in w.Win32_ComputerSystem():
        print("[+] Manufacturer:", sys.Manufacturer)
        print("    Model:", sys.Model)
        print("    Name:", sys.Name)
        print("    System Type:", sys.SystemType)
        print("    Total Physical Memory:", int(sys.TotalPhysicalMemory) // (1024 ** 2), "MB")
        print()

def get_display_info(w):
    print("\n==== DISPLAY INFO ====")
    for disp in w.Win32_DesktopMonitor():
        print("[+] Name:", disp.Name)
        print("    Monitor Type:", disp.MonitorType)
        print("    Screen Height:", disp.ScreenHeight)
        print("    Screen Width:", disp.ScreenWidth)
        print("    Device ID:", disp.DeviceID)
        print()

print("==== SYSTEM INFORMATION TOOL ====")
w = wmi.WMI()

get_os_info()
get_system_info(w)
get_cpu_info(w)
get_gpu_info(w)
get_ram_info()
get_disk_info(w)
get_network_info()  # Public IP and network interfaces info
get_display_info(w)
get_motherboard_info(w)
get_bios_info(w)
