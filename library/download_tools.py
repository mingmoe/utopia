
import urllib.request
import zipfile
import os

# only called under windows
def utopia_download_tools():
    SCRIPT_ROOT = os.path.dirname(os.path.realpath(__file__))
    DOWNLOAD_ROOT = os.path.join(SCRIPT_ROOT,"tools/")

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
        out_dir = os.path.join(DOWNLOAD_ROOT,opt)
        source_file = get_download_path(path)

        if not os.path.exists(out_dir) or not os.path.isdir(out_dir):
            with zipfile.ZipFile(source_file, 'r') as zip_ref:
                print(f"extract {source_file} to {out_dir}")
                zip_ref.extractall(out_dir)


    download_file("https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip","nasm.zip")
    download_file("https://strawberryperl.com/download/5.32.1.1/strawberry-perl-5.32.1.1-64bit-portable.zip","perl.zip")

    extract_file("nasm.zip","nasm")
    extract_file("perl.zip","perl")

if __name__  == "__main__":
    utopia_download_tools()
