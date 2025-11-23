import serial
import serial.tools.list_ports
import time
import socket
import sys

# --- CONFIGURARE ---
BAUD_RATE = 38400 
AUTHORIZED_HOST = "Admin"
# -------------------

def print_banner():
    print(r"""
   _____           _      _       _    
  / ____|         | |    (_)     | |   
 | (___  _   _ ___| |     _ _ __ | | __
  \___ \| | | / __| |    | | '_ \| |/ /
  ____) | |_| \__ \ |____| | | | |   < 
 |_____/ \__, |___/______|_|_| |_|_|\_\
          __/ |                        
         |___/                         
      [ SysLink Console v1.0 ]
      [ Industrial IoT Gateway ]
    """)

def print_help():
    print("\n=== AVAILABLE COMMANDS ===")
    print(" [PWM]    POST /pwm/set/<channel>/<duty>   (ch: 0-1, duty: 0-255)")
    print(" [GPIO]   POST /gpio/toggle/<port>/<pin>   (ex: B/5)")
    print(" [GPIO]   POST /gpio/set/<port>/<pin>/<lv> (lv: high/low)")
    print(" [ADC]    GET /adc/read/<channel>          (ch: 0-7)")
    print(" [SYS]    GET /status                      (Full System Report)")
    print(" [SYS]    GET /msg                         (Check Functionality)")
    print(" [CLI]    help, exit, clear")
    print("==========================\n")

def select_port():
    print("Scanning for serial ports...")
    ports = list(serial.tools.list_ports.comports())
    
    if not ports:
        print("[!] No serial ports found. Check connection.")
        input("Press Enter to exit...")
        sys.exit()
        
    for i, p in enumerate(ports):
        print(f"  {i+1}. {p.device} - {p.description}")
    
    if len(ports) == 1:
        print(f"[+] Auto-selected: {ports[0].device}")
        return ports[0].device
        
    while True:
        try:
            selection = input("\nSelect port (number): ").strip()
            idx = int(selection) - 1
            if 0 <= idx < len(ports):
                return ports[idx].device
        except:
            pass
        print("Invalid selection.")

def main():
    print_banner()
    port_name = select_port()
    
    try:
        print(f"[...] Connecting to {port_name} at {BAUD_RATE} baud...")
        ser = serial.Serial(port_name, BAUD_RATE, timeout=1)
        time.sleep(2)
        print("[+] CONNECTION ESTABLISHED")
        print(f"[i] Authorized Security Host: '{AUTHORIZED_HOST}'")
        print("[i] Type 'help' for command list.\n")
        
    except Exception as e:
        print(f"\n[!] Connection Failed: {e}")
        input("Press Enter to exit...")
        sys.exit()

    while True:
        try:
            cmd = input("SysLink > ").strip()
            
            if not cmd: continue
            
            if cmd.lower() == "exit": break
            if cmd.lower() == "help": print_help(); continue
            if cmd.lower() == "clear": print("\n" * 50); continue

            request = f"{cmd} HTTP/1.1\r\nHost: {AUTHORIZED_HOST}\r\n\r\n"
            
            ser.write(request.encode())
            
            response = ""
            while True:
                line = ser.readline().decode('utf-8', errors='ignore')
                if not line: break
                response += line
            
            print(f"\n{response.strip()}\n")
            
        except serial.SerialException:
            print("\n[!] Device disconnected.")
            break
        except KeyboardInterrupt:
            break

    ser.close()
    print("[*] Session closed.")

if __name__ == "__main__":
    main()