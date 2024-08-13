import * as fs from "fs";
import {join} from "path";
import * as http from "http";
import {zip_all, zip_selected} from "./zip"

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

function get_component_config(name: string): [COMPONENT, "json" | "yaml"]
{
    let config = join(name, "oul.config.json");
    let format = "json";
    if (!fs.existsSync(config))
    {
        format = "yaml";
        config = join(name, "oul.config.yaml");
        
    }
    return [JSON.parse(fs.readFileSync(config).toString()), format as "json" | "yaml"];
}

function on_get_request({path, queries, original}: PARSED_URL, res: http.ServerResponse)
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
        let what = queries.get("what").split(",").sort();
        let zip_file = name + "." + what.join("_") + ".zip";

        if (fs.existsSync(zip_file))
        {
            let component = fs.readFileSync(zip_file);
            res.writeHead(200, {'Content-Type': 'application/zip'});
            res.end(component);
            log_request(original, 200);
        }
        else if (fs.existsSync(name))
        {
            if (what.length == 0)
            {
                zip_all(name, zip_file);
            }
            else
            {
                let [component, format] = get_component_config(name);
                if (!what.includes("src"))
                {
                    component.source_files = [];
                }
                if (!what.includes("test"))
                {
                    component.test_files = [];
                }
                if (!what.includes("doc"))
                {
                    component.documentation = [];
                }

                zip_selected(name, zip_file, component, format);

                res.writeHead(200, {'Content-Type': 'application/zip'});
                res.end(fs.readFileSync(zip_file));
                log_request(original, 200);
            }
        }
        else
        {
            res.writeHead(404, {'Content-Type': 'text/plain'});
            res.end("Component not found.");
            log_request(original, 404);
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
