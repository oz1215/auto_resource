auto_resource
=============

C++(特にWin32API)においてリソースの開放を自動的に行うクラス

使い方の例        
        int main()
        {
                AutoResource<HMODULE> r1(::LoadLibrary(...),      ::FreeLibrary);
                AutoResource<HBRUSH>  r2(::CreateSolidBrush(...), ::DeleteObject);
                AutoResource<HANDLE>  r3(::CreateFile(...),       ::CloseHandle);

                AutoHandle h(static_cast<HANDLE>(0));

                return 0;
        }
