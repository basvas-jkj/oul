import * as http from "http";
import on_request from "./server";

function on_listen(): void
{
    console.log("Server se spustil.");
}
function on_server_error(error): void
 {
     console.warn(error.message);
 }

function main(args: string[]): void
{
    let port = 8080;
    for (let arg of args)
    {
        if (arg.startsWith("-port"))
        {
            port = Number.parseInt(arg.split("=")[1]);
        }
    }

    const server = http.createServer(on_request);
    server.listen(port, '127.0.0.1', on_listen);
    server.on("error", on_server_error);
}

main(process.argv);
