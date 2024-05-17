import http.server
import socketserver

class HttpRequestHandler(http.server.SimpleHTTPRequestHandler):
    extensions_map = {
        ".html" : "text/html",
        ".css"  : "text/css",
        ".js"   : "application/javascript",
        ".json" : "application/json",
        ".wasm" : "application/wasm",
        ".png"  : "image/png",
        ".jpg"  : "image/jpg",
        ".svg"  : "image/svg+xml",
        ".xml"  : "application/xml",
        ""      : "application/octet-stream"
    }

host = "localhost"
port = 8001

httpd = socketserver.TCPServer((host, port), HttpRequestHandler)

try:
    print(f"serving at http://{host}:{port}")
    httpd.serve_forever()
except KeyboardInterrupt:
    pass
