import * as fs from "fs";
import * as AdmZip from "adm-zip";

function zip_all(component_folder: string, output_file: string)
{
    let zip = new AdmZip();
    zip.addLocalFolder(component_folder);
    zip.writeZip(output_file);
}
export function zip_component(component_folder: string)
{
    let zip_file = component_folder + ".zip";
    if (fs.existsSync(zip_file))
    {
        return zip_file;
    }
    else if (fs.existsSync(component_folder))
    {
        zip_all(component_folder, zip_file);
        return zip_file;
    }
    else
    {
        return null;
    }
}
export function unzip_component(component_folder: string, input_file: string)
{
    let zip = new AdmZip(input_file);
    zip.extractAllTo(component_folder);
}