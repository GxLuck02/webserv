import threading
import socket
import time

HOST = "127.0.0.1"
PORTS = [8080, 9090]
REQUEST = b"GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n"
TIMEOUT = 5  # Timeout socket côté client

# === CLIENT DE BASE ===
def test_connection(port, index):
    try:
        print(f"[Client {index}] Connecting to port {port}")
        with socket.create_connection((HOST, port), timeout=TIMEOUT) as sock:
            sock.sendall(REQUEST)
            response = b""
            while True:
                chunk = sock.recv(4096)
                if not chunk:
                    break
                response += chunk
        if b"HTTP/1.1" in response:
            print(f"[Client {index}] ✅ Response OK from port {port}")
        else:
            print(f"[Client {index}] ❌ Invalid response from port {port}")
    except Exception as e:
        print(f"[Client {index}] ❌ Error: {e}")

# === CLIENT INACTIF ===
def test_inactive_client(port, sleep_time, index):
    try:
        print(f"[Inactive Client {index}] Connecting to port {port}")
        with socket.create_connection((HOST, port), timeout=TIMEOUT) as sock:
            print(f"[Inactive Client {index}] Connected. Sleeping {sleep_time}s...")
            time.sleep(sleep_time)  # Simule un client inactif
            try:
                sock.sendall(REQUEST)
                response = sock.recv(1024)
                if not response:
                    print(f"[Inactive Client {index}] ✅ Timed out as expected.")
                else:
                    print(f"[Inactive Client {index}] ❌ Still active. Unexpected.")
            except Exception:
                print(f"[Inactive Client {index}] ✅ Socket closed by server (timeout expected).")
    except Exception as e:
        print(f"[Inactive Client {index}] ❌ Error on connect: {e}")

# === LANCER LES TESTS ===
def run_stress_test():
    print("\n🚀 Starting STRESS TEST with 100 clients...\n")
    threads = []
    for i in range(100):
        port = PORTS[i % len(PORTS)]
        t = threading.Thread(target=test_connection, args=(port, i))
        t.start()
        threads.append(t)

    for t in threads:
        t.join()
    print("✅ Stress test completed.\n")

def run_inactivity_test():
    print("\n⏳ Starting TIMEOUT TEST with inactive clients...\n")
    threads = []

    # 2 clients sur chaque port, qui dorment + que leur timeout
    for i, port in enumerate(PORTS):
        timeout = 65 if port == 8080 else 55  # Sleep > timeout pour forcer la fermeture côté serveur
        for j in range(2):
            t = threading.Thread(target=test_inactive_client, args=(port, timeout, f"{port}_{j}"))
            t.start()
            threads.append(t)

    for t in threads:
        t.join()
    print("✅ Inactivity test completed.\n")

if __name__ == "__main__":
    start = time.time()
    run_stress_test()
    run_inactivity_test()
    print(f"🧪 All tests done in {time.time() - start:.2f} seconds.")
