import * as fs from "fs";
import {join} from "path";
import * as AdmZip from "adm-zip";
import type {COMPONENT} from "./server"

export function zip_all(component_folder: string, output_file: string)
{
    let zip = new AdmZip();
    zip.addLocalFolder(component_folder);
    zip.writeZip(output_file);
}
export function zip_selected(component_folder: string, output_file: string, component_config: COMPONENT, format: "json" | "yaml"): void
{
    let source = component_config.source_files;
    let tests = component_config.test_files;
    let docs = component_config.documentation;

    let zip = new AdmZip();
    for (let entry of source.concat(tests, docs))
    {
        let full_path = join(component_folder, entry);
        if (fs.statSync(full_path).isDirectory())
        {
            zip.addLocalFolder(full_path, entry);
        }
        else
        {
            zip.addLocalFile(full_path);
        }
    }

    let buf = Buffer.from(JSON.stringify(component_config));
    zip.addFile("oul.component." + format, buf);
    zip.writeZip(output_file);
}