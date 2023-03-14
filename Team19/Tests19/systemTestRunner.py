import os
import sys
import subprocess
import webbrowser
 

# ensure that .exe has been built before running script

rootDIR = (os.path.dirname(os.path.realpath(__file__)))
os.chdir(rootDIR)
print(os.getcwd())

releasePath = f"..\\Code19\\Release\\AutoTester.exe"
debugPath = f"..\\Code19\\Debug\\AutoTester.exe"



if len(sys.argv) > 1:
    usingPath = releasePath
else:
    usingPath = debugPath


print(f"using {usingPath}")

if os.path.exists(usingPath):
    for root, dirs, files in os.walk(f"..\\out_xml"):
        for file in files:
            f = os.path.join(root,file)
            if(f.endswith(".xml")):
                os.remove(f)

    for root, dirs, files in os.walk(f".\\systemTests"):
        for dir in dirs:
            source = os.path.join(root, dir, f"{dir}_source.txt")
            query = os.path.join(root, dir, f"{dir}_query.txt")
            out = os.path.join(root, "..\\","out_xml" ,f"{dir}_out.xml")        
            # print(f"{usingPath} {source} {query} {out}")
            os.system(f"{usingPath} {source} {query} {out}")


    print(f"displaying results for {usingPath}")
    webbrowser.open("http://localhost:8000/out_xml")    
    subprocess.run("python -m http.server 8000")
else:
    print(f"path not found : {usingPath}")