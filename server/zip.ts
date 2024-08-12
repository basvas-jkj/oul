import * as fs from "fs";
import {join} from "path";
import * as AdmZip from "adm-zip";

function ignore(name: string, files: string[]): boolean
{
    return !files.includes(name) && name != "oul.component.json" && name != "oul.component.yaml";
}

export function create_zip_archive(component_folder: string, output_file: string, files: string[]): void
{
    let zip = new AdmZip();
    let entries = fs.readdirSync(component_folder);
    for (let entry of entries)
    {
        let full_path = join(component_folder, entry);
        if (ignore(entry, files))
        {
            continue;
        }
        else if (fs.statSync(full_path).isDirectory())
        {
            zip.addLocalFolder(full_path, entry);
        }
        else
        {
            zip.addLocalFile(full_path);
        }
    }

    zip.writeZip(output_file);
}