Lista de Connectores pré-definidos
----------------------------------
Ao criar um novo projeto no {nclcomposer} é possível adicionar uma base de
conectores pré-definidos (_defaultConnBase.ncl_). Esta base de conectores
pré-definidos traz um conjunto de 


[width="100%",frame="topbot",options="header"]
|=======
|          | *start*      | *stop*      | *pause*      | *resume*      | *set* + parâmetro _var_
|*onBegin* | onBeginStart | onBeginStop | onBeginPause | onBeginResume | onBeginSet
|*onEnd*   | onEndStart   | onEndStop   | onEndPause   | onEndResume   | onEndSet
|*onSelection* (seleção por mouse) | onSelectionStart  | onSelectionStop |
onSelectionPause | onSelectionResume | onSelectionSet
|*onSelection* + parâmetro _key_ (seleção por tecla) | onKeySelectionStart |
onKeySelectionStop | onKeySelectionPause | onKeySelectionResume |
onKeySelectionSet
|=======

* TODO: Como definir minha própria base de conectores???
* TODO: Como editar minha base de conectores default??