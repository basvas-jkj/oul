import * as fs from "node:fs";
import * as http from "node:http";

type PARSED_URL = 
{
    path: string[];
    queries: URLSearchParams;

}
function parse_url(url: string): PARSED_URL
{
    let url_object = new URL("http://locahost" + url);
    let split_path = url_object.pathname.split("/");
    url_object.searchParams
    split_path.shift();
    return {
        path: split_path,
        queries: url_object.searchParams
    }
}
function log_request(url: string, status: number)
{
    console.log("Server dostal požadavek:", url);
    console.log("\tstatus:", status);
}

function on_listen()
{
    console.log("Server se spustil.");
}
function on_request(req: http.IncomingMessage, res: http.ServerResponse)
{
    let {path} = parse_url(req.url);
    
    if (path.length == 0 || path[0] == "")
    {
        res.writeHead(400, "Missing component name.");
        res.end();
        log_request(req.url, 400);
    }
    else if (path.length == 1)
    {
        let name = path[0];
        try
        {
            let component = fs.readFileSync(name + ".zip");
            res.writeHead(200, {'Content-Type': 'application/zip'});
            res.end(component);
            log_request(req.url, 200)
        }
        catch (error)
        {
            res.writeHead(404, {'Content-Type': 'application/zip'});
            res.end(JSON.stringify(error));
            log_request(req.url, 404);
        }

    }
    else
    {
        res.writeHead(400, "Path should contain component name only.");
        res.end();
        log_request(req.url, 400);
    }
}
function on_server_error(error)
 {
     console.warn(error.message);
 }


function main()
{
    const server = http.createServer(on_request);
    server.on("error", on_server_error);
    server.listen(8080, '127.0.0.1', on_listen);
}

main();
