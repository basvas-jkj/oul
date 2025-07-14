import * as http from "http";
import on_request from "./server";

function on_listen(ip: string, port: number): void
{
    console.log(`Server se spustil (na ${ip}:${port}).`);
}
function on_server_error(error: Error): void
 {
     console.warn(error.message);
 }

function main(args: string[]): void
{
    let port = 8080;
    let ip = '127.0.0.1';
    for (let arg of args)
    {
        if (arg.startsWith("-port"))
        {
            port = Number.parseInt(arg.split("=")[1]);
        }
        else if (arg.startsWith("-ip"))
        {
            ip = arg.split("=")[1];
        }
    }

    const server = http.createServer((req, res)=>on_request(req, res));
    server.listen(port, ip, ()=>on_listen(ip, port));
    server.on("error", on_server_error);
}

main(process.argv);
