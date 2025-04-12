import * as fs from "fs";
import {join} from "path";
import * as http from "http";
import {zip_all, zip_selected} from "./zip";
import {hash} from "crypto";

import type {USER_INFO, SERVER_CONFIG} from "./index"

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
    let config = join(name, "oul.component.json");
    let format = "json";
    if (!fs.existsSync(config))
    {
        format = "yaml";
        config = join(name, "oul.component.yaml");
        
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
        let what = queries.get("what")?.split(",")?.sort();
        let zip_file = (what == null) ?  name + ".zip" : name + "." + what.join("_") + ".zip";

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
function on_put_request({path, queries, original}: PARSED_URL, req: http.IncomingMessage, res: http.ServerResponse, config: SERVER_CONFIG)
{
    if (path.length == 1 && path[0] == "register")
    {
        let content = "";
        
        function on_data(chunk: string)
        {
            content += chunk;
        }
        function on_end()
        {
            const [username, password] = content.split(":", 2);
            function predicate(user: USER_INFO)
            {
                return user.name == username;
            }

            if (password == undefined || password == "")
            {
                res.writeHead(403);
                res.end("No password specified.");
                log_request(original, 403);
            }
            else
            {
                let user: USER_INFO =
                {
                    admin: queries.has("admin", "true"),
                    name: username,
                    hashed_password: hash("sha256", password)
                };

                if (config.users.find(predicate) == undefined)
                {
                    config.users.push(user);
                    fs.writeFileSync("oul.server.json", JSON.stringify(config));
                }

                res.writeHead(204);
                res.end();
                log_request(original, 204);
            }
        }

        req.on("data", on_data);
        req.on("end", on_end);

    }
    else
    {
        res.writeHead(400);
        res.end("Unknown request.");
        log_request(original, 400);
    }
}

function reject_authorization(method: string, authorization: string, config: SERVER_CONFIG): boolean
{
    if ((method == "GET" && !config.get_requires_authorization) || (method == "POST" && !config.post_requires_authorization))
    {
        return false;
    }
    else if (authorization == undefined)
    {
        return true;
    }

    const auth = atob(authorization.split(" ", 2)[1]);
    const [username, password] = auth.split(":");

    function predicate(user: USER_INFO)
    {
        return user.name == username && user.hashed_password == hash("sha256", password);
    }
    const authorized_user = config.users.find(predicate);

    if (method == "PUT")
    {
        return authorized_user == undefined || !authorized_user.admin;
    }
    else 
    {
        return authorized_user == undefined;
    }
}
export default function on_request(req: http.IncomingMessage, res: http.ServerResponse, config: SERVER_CONFIG): void
{
    let url = parse_url(req.url);

    if (reject_authorization(req.method, req.headers.authorization, config))
    {
        res.writeHead(401);
        res.end("Rejected.");
        log_request(req.url, 401);
    }
    else if (req.method == "GET")
    {
        on_get_request(url, res);
    }
    else if (req.method == "PUT")
    {
        on_put_request(url, req, res, config);
    }
    else
    {
        res.writeHead(501);
        res.end("This method is not supported.");
        log_request(req.url, 501);
    }
}
