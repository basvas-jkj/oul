import * as fs from "fs";
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
    url_object.searchParams
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

function on_get_request({path, original}: PARSED_URL, res: http.ServerResponse)
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
export default function on_request(req: http.IncomingMessage, res: http.ServerResponse): void
{
    let url = parse_url(req.url);
    
    if (req.method == "GET")
    {
        on_get_request(url, res);
    }
    else
    {
        res.writeHead(501);
        res.end("This method is not supported.");
        log_request(req.url, 501);
    }
}
