import * as fs from "fs";
import * as path from "path";
import * as http from "http";
import {zip_component, unzip_component} from "./zip";

type PARSED_URL = 
{
    path: string[];
    queries: URLSearchParams;
    original: string;
};
export type COMPONENT =
{
    name: string,
    repository: {url: string},
    source_files: string[],
    test_files: string[],
    documentation: string[]
};

function parse_url(url: string): PARSED_URL
{
    let url_object = new URL("http://locahost" + url);
    let split_path = url_object.pathname.split("/");
    split_path.shift();
    return {
        path: split_path,
        queries: url_object.searchParams,
        original: url
    }
}
function log_request(url: string, status: number): void
{
    console.log("Server dostal požadavek:", url);
    console.log("\tstatus:", status);
}
async function retrieve_full_body(req: http.IncomingMessage): Promise<Buffer>
{
    return new Promise<Buffer>((resolve, reject)=>
    {
        let body = Buffer.from("");
        req.on("data", (chunk)=>
        {
            body = Buffer.concat([body, chunk]);
        });
        req.on("end", ()=>resolve(body));
        req.on("error", (error)=>reject(error));
    });
}
function is_component(entry: fs.Dirent): boolean
{
    if (!entry.isDirectory)
    {
        return false;
    }
    let component_config_file = path.join(entry.parentPath, entry.name, "oul.component.json");
    let stats = fs.statSync(component_config_file, {throwIfNoEntry: false});

    if (stats == undefined)
    {
        return false;
    }
    else
    {
        return stats.isFile();
    }
}
function list_all_components(): string[]
{
    let components = [] as string[];
    let entries = fs.readdirSync(".", {withFileTypes: true});
    for (let entry of entries)
    {
        if (is_component(entry))
        {
            components.push(entry.name);
        }
    }
    return components;
}

function on_get_request({path, original}: PARSED_URL, res: http.ServerResponse)
{
    if (path.length == 0 || path[0] == "")
    {
        let components = list_all_components();
        res.writeHead(200);
        res.end(JSON.stringify(components));
        log_request(original, 200);
    }
    else if (path.length == 1)
    {
        let name = path[0];
        let zip_file = zip_component(name);

        if (zip_file == null)
        {
            res.writeHead(404, {'Content-Type': 'text/plain'});
            res.end("Component not found.");
            log_request(original, 404);
        }
        else
        {
            let component = fs.readFileSync(zip_file);
            res.writeHead(200, {'Content-Type': 'application/zip'});
            res.end(component);
            log_request(original, 200);
        }
    }
    else
    {
        res.writeHead(400);
        res.end("Path should contain component name only.");
        log_request(original, 400);
    }
}
function on_post_request({path, original}: PARSED_URL, body: Buffer, res: http.ServerResponse)
{
    if (path.length == 0 || path[0] == "")
    {
        res.writeHead(400);
        res.end("Missing component name.");
        log_request(original, 400);
    }
    else if (path.length == 1)
    {
        let name = path[0];
        let zip_file = name + ".zip";
        
        fs.writeFileSync(zip_file, body);
        
        try
        {
            unzip_component(name, zip_file);
            res.writeHead(201);
            res.end();
            log_request(original, 201);
        }
        catch (err: any)
        {
            console.warn(err);
            res.writeHead(400);
            res.end(err.toString());
            log_request(original, 400);
        }
    }
    else
    {
        res.writeHead(400);
        res.end("Path should contain component name only.");
        log_request(original, 400);
    }
}

export default async function on_request(req: http.IncomingMessage, res: http.ServerResponse): Promise<void>
{
    try
    {
        let url = parse_url(req.url);

        if (req.method == "GET")
        {
            on_get_request(url, res);
        }
        else if (req.method == "POST")
        {
            let body = await retrieve_full_body(req);
            on_post_request(url, body, res);
        }
        else
        {
            res.writeHead(501);
            res.end("This method is not supported.");
            log_request(req.url, 501);
        }
    }
    catch (e: unknown)
    {
        let message = (e instanceof Error) ? e.message : (e?.toString() ?? e);
        res.writeHead(500);
        res.end(message);
        log_request(req.url, 500);
    }
}
