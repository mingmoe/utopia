
import urllib.request
import zipfile
import os

SCRIPT_ROOT = os.path.dirname(os.path.realpath(__file__))
DOWNLOAD_ROOT = os.path.join(SCRIPT_ROOT,"temp/")

os.makedirs(DOWNLOAD_ROOT,exist_ok=True)


def get_download_path(file):
    return os.path.join(DOWNLOAD_ROOT,file)


def download_file(url,output):
    if not os.path.exists(get_download_path(output)) or not os.path.isfile(get_download_path(output)):
        print(f"download {url} to {get_download_path(output)}")
        with urllib.request.urlopen(url) as f:
            with open(get_download_path(output),"wb") as fs:
                fs.write(f.read())


def extract_file(path,opt):
    out_dir = os.path.join(SCRIPT_ROOT,opt)
    source_file = get_download_path(path)
    with zipfile.ZipFile(source_file, 'r') as zip_ref:
        print(f"extract {source_file} to {out_dir}")
        zip_ref.extractall(out_dir)


download_file("https://github.com/unicode-org/icu/releases/download/release-71-1/icu4c-71_1-data-bin-l.zip","icu4c-data-l.zip")
download_file("https://github.com/unicode-org/icu/releases/download/release-71-1/icu4c-71_1-data-bin-b.zip","icu4c-data-b.zip")

extract_file("icu4c-data-l.zip","icu4c-data")
extract_file("icu4c-data-b.zip","icu4c-data")
