import * as fs from "fs";
import * as http from "http";
import on_request from "./server";

export type SERVER_CONFIG =
{
    port: number,
    get_requires_authorization: boolean,
    post_requires_authorization: boolean,
    users:
    {
        name: string
        hashed_password: string
    }[]
}

function on_listen(port: number): void
{
    console.log(`Server se spustil (na portu ${port}).`);
}
function on_server_error(error): void
 {
     console.warn(error.message);
 }

function read_server_configuration(): SERVER_CONFIG
{
    try
    {
        const buf = fs.readFileSync("oul.server.json");
        return JSON.parse(buf.toString());
    }
    catch (err)
    {
        console.warn(err.message);
        process.exit(1);
    }
}
function main(args: string[]): void
{
    const config = read_server_configuration();
    let port = config.port;
    for (let arg of args)
    {
        if (arg.startsWith("-port"))
        {
            port = Number.parseInt(arg.split("=")[1]);
        }
    }

    const server = http.createServer(on_request);
    server.listen(port, '127.0.0.1', ()=>on_listen(port));
    server.on("error", on_server_error);
}

main(process.argv);
