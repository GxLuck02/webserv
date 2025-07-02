import asyncio
import time
import random

HOST = '127.0.0.1'
PORT = 8080
CLIENT_COUNT = 500
MESSAGE = b"Hello from client!\r\n\r\n"

# Comportements possibles, sans "no_close" maintenant
CLIENT_TYPES = ["normal", "fire_and_forget", "slow_response", "invalid_data"]

async def tcp_client(id, behavior):
    try:
        reader, writer = await asyncio.open_connection(HOST, PORT)

        if behavior == "invalid_data":
            writer.write(b"\x00\x01\x02\x03INVALID\r\n\r\n")
        else:
            writer.write(MESSAGE)
        await writer.drain()

        if behavior == "fire_and_forget":
            # Envoie et ferme immédiatement sans lire la réponse
            writer.close()
            await writer.wait_closed()
            print(f"[Client {id} - {behavior}] ✅ Sent and closed immediately.")
            return

        if behavior == "slow_response":
            await asyncio.sleep(random.uniform(1, 3))  # Attente avant lecture

        # Lecture de la réponse avec timeout 5s
        data = await asyncio.wait_for(reader.read(1024), timeout=5)

        if behavior == "normal":
            if data != MESSAGE:
                print(f"[Client {id} - {behavior}] ❌ Echo mismatch!")
            else:
                print(f"[Client {id} - {behavior}] ✅ Received echo OK")
        else:
            print(f"[Client {id} - {behavior}] ✅ Received data (length {len(data)})")

        writer.close()
        await writer.wait_closed()

    except Exception as e:
        print(f"[Client {id} - {behavior}] ❌ Error: {e}")

async def main():
    print(f"🚀 Launching {CLIENT_COUNT} clients with mixed behaviors...")
    start = time.time()

    tasks = []
    for i in range(CLIENT_COUNT):
        behavior = random.choice(CLIENT_TYPES)
        tasks.append(tcp_client(i, behavior))

    await asyncio.gather(*tasks)

    duration = time.time() - start
    print(f"\n✅ All clients finished in {duration:.2f} seconds.")

if __name__ == "__main__":
    asyncio.run(main())
