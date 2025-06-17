import asyncio
import time

HOST = '127.0.0.1'
PORT = 8080
CLIENT_COUNT = 120
MESSAGE = b"Hello from client!"

async def tcp_client(id):
    try:
        reader, writer = await asyncio.open_connection(HOST, PORT)
        writer.write(MESSAGE)
        await writer.drain()

        data = await asyncio.wait_for(reader.read(1024), timeout=5)
        if data != MESSAGE:
            print(f"[Client {id}] ❌ Echo mismatch!")
        else:
            print(f"[Client {id}] ✅ Received echo OK")

        writer.close()
        await writer.wait_closed()
    except Exception as e:
        print(f"[Client {id}] ❌ Error: {e}")

async def main():
    print(f"🚀 Launching {CLIENT_COUNT} clients...")
    start = time.time()

    tasks = [tcp_client(i) for i in range(CLIENT_COUNT)]
    await asyncio.gather(*tasks)

    duration = time.time() - start
    print(f"\n✅ All clients finished in {duration:.2f} seconds.")

if __name__ == "__main__":
    asyncio.run(main())
