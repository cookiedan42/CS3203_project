# System test runner  

Create system test in a subfolder such that  
- {TestName}
    - {TestName}_source.txt  
    - {TestName}_query.txt  
  
Build the autotester.exe file that is to be tested  
Run systemTestRunner.py  
    - running with no arguments will launch debug build  
    - running with any arguments will launch release build  
The test runner will run all the system tests and output the xml files as `out_xml/{TestName}_out.xml`, then launch the viewing server and web browser to display the tests  
  
`demo_source.txt` and `demo_query.txt` in the `Tests19` folder are for visual studio autotester debugger to use  