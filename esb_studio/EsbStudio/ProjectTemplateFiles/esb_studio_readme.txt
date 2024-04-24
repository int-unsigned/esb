ВАЖНО
- Имя файла 1с-модуля должно быть АНГЛИЙСКИМ !!!
	!!! Иначе студия в .vcxproj и в .vcxproj.filters КАКУЮ-ТО ЕРЕСЬ пишет !!!
	!!! Нужно в M$ кляузу писать !!!
	Это именно про файл (напиример) EsbComponent.xml, 
	его папку EsbComponent, и его запись в Configuration.xml <CommonModule>EsbComponent</CommonModule>
	А в самом файле можно написать <Name>ЕсбКомпонент</Name> и в 1С он будет по-русски
	Т.е. чтобы запись о файле в .vcxproj и в .vcxproj.filters была
  <ItemGroup>
    <OneEsFiles Include="1s_files\CommonModules\EsbComponent\Ext\Module.bsl">
      <Filter>1С Файлы\Общие модули\EsbComponent</Filter>
    </None>
  </ItemGroup>
ПС. Хотя хз... раза три отработало, когда файл-по-английски, а имя-модуля-по-русски, а потом перестало...
	Что там в потрохах 1С творится - загадка великая..
	Все по-английски надежнее!
