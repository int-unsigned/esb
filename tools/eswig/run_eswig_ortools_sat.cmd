@echo off
eswig.exe -I- -ID:\Dev\esb-dev\esb\tools\eswig\Lib\.swig-4.3.1 -ID:\Dev\esb-dev\esb\tools\eswig\Lib\esb -ID:\Dev\esb-dev\esb\tools\eswig\modules\google -ID:\dev\.Pkg\or-tools\9.11\include -c++ -Wall -cpperraswarn -xml -chkfile ..\..\modules\esb\modules\ortools\ortools_sat.hpp modules\google\ortools\ortools_sat.swp

pause