@REM cmake -B build -G "Visual Studio 17" -DLY_PROJECTS="AtomSampleViewer;AutomatedTesting" -DLY_RENDERDOC_ENABLED=ON -DLY_PIX_ENABLED=ON -DLY_PIX_PATH="C:\winpixeventruntime.1.0.240308001" 
@REM cmake -B build -G "Visual Studio 17" -DLY_PROJECTS="AutomatedTesting" -DLY_RENDERDOC_ENABLED=ON -DLY_PIX_ENABLED=ON -DLY_PIX_PATH="C:\winpixeventruntime.1.0.240308001"
@REM cmake -B build -G "Visual Studio 17" -DLY_RENDERDOC_ENABLED=ON -DLY_PIX_ENABLED=ON -DLY_PIX_PATH="C:\winpixeventruntime.1.0.240308001"

cmake -B build -G "Visual Studio 17" -DLY_RENDERDOC_ENABLED=ON
