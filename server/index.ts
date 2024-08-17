import * as fs from "fs";
import {hash} from "crypto";
import * as http from "http";
import {createInterface} from "readline/promises";
import on_request from "./server";

export type USER_INFO =
{
    admin: boolean;
    name: string;
    hashed_password: string;
}
export type SERVER_CONFIG =
{
    port: number;
    get_requires_authorization: boolean;
    post_requires_authorization: boolean;
    users: USER_INFO[];
}

function on_listen(port: number): void
{
    console.log(`Server se spustil (na portu ${port}).`);
}
function on_server_error(error): void
 {
     console.warn(error.message);
 }

async function initialize()
{
    let i = createInterface(process.stdin, process.stdout);

    let name = await i.question("Username (admin): ");
    let password = "";
    while (password == "")
    {
        password = await i.question("Password: ");
    }
    let port = await i.question("Default port (80): ");
    let get = await i.question("GET requires authorization (no): ");
    let post = await i.question("POST requires authorization (yes): ");
    
    let config: SERVER_CONFIG =
    {
        port: (port == "") ? 80 : Number.parseInt(port),
        get_requires_authorization: get.toLowerCase().startsWith('y'),
        post_requires_authorization: !post.toLowerCase().startsWith('n'),
        users:
        [{
            admin: true,
            name: (name == "") ? "admin" : name,
            hashed_password: hash("sha256", password)
        }]
    };
    
    if (fs.existsSync("oul.server.json"))
    {
        console.log("\noriginal:", JSON.parse(fs.readFileSync("oul.server.json").toString()));
        console.log("new", config)
        let response = await i.question("The server configuration has been already created. Do you want to override it? (no)");
        if (!response.toLowerCase().startsWith("y"))
        {
            return;
        }
    }

    fs.writeFileSync("oul.server.json", JSON.stringify(config));
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
    if (args.includes("init"))
    {
        initialize().then(()=>process.exit(0));
        return;
    }

    const config = read_server_configuration();
    let port = config.port;
    for (let arg of args)
    {
        if (arg.startsWith("-port"))
        {
            port = Number.parseInt(arg.split("=")[1]);
        }
    }

    const server = http.createServer((req, res)=>on_request(req, res, config));
    server.listen(port, '127.0.0.1', ()=>on_listen(port));
    server.on("error", on_server_error);
}

main(process.argv);
