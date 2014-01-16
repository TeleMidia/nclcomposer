
[[sec:windows_and_menus]]
Janelas e Menus do NCL Composer
-------------------------------
Janela Principal
~~~~~~~~~~~~~~~~

image:../imgs/cap4_mainwindow.png[width=300px]

Barra de título::
Evidencia o nome do {nclcomposer} e do projeto corrente.

Barra de menu::
Mostra as entradas de menu disponíveis.

Barra de ferramentas::
As ações globais ficam disponíveis na barra de ferramentas. Por exemplo, as
açoes para salvar o projeto atual, executá-lo etc.

Tab Bar::
Lista a tela de boas-vindas e a lista de projetos abertos, permitindo navegar
entre eles.

Área das Visões::
Área de desenho das diversas <<def:views, Visões>>. Cada visão pode ter
seus próprios elementos de interface. Também é possível organizar e salvar a
disposição das diversas visões por meio das <<def:perspective, Perspectivas>>.

Visões _Default_
~~~~~~~~~~~~~~~~
Por padrão o NCL Composer vem com as seguintes visões:

[[sec:structural_view]]
Visão Estrutural
^^^^^^^^^^^^^^^^
A {structural_view} permite que os autores de aplicações interajam visualmente
com a estrutura lógica do documento NCL. Em NCL, a estrutura lógica é
representada pelos seus objetos (nós de conteúdo e nós de composição) e os
elos entre esses objetos (representadas pelos elementos <link>).

image::http://composer.telemidia.puc-rio.br/_media//plugins/structural-plugin-02102012.png[title="{structural_view}",width="35%",link="http://composer.telemidia.puc-rio.br/_media//plugins/structural-plugin-02102012.png"]

[[sec:layout_view]]
Visão de Leiaute
^^^^^^^^^^^^^^^^
A {layout_view} permite que os autores de aplicações NCL interajam visualmente
com as regiões onde os objetos de mídia serão inicialmente apresentados. Nesta
visão, os autores podem criar e manipular as regiões e a hierarquia de regiões
do documento.

image::http://composer.telemidia.puc-rio.br/_media/layout_view.png[title="{layout_view}",width="35%",link="http://composer.telemidia.puc-rio.br/_media/layout_view.png"]

[[sec:outline_view]]
Visão de Outline
^^^^^^^^^^^^^^^^
A {outline_view} apresenta a estrutura do documento NCL como uma árvore,
permitindo aos autores navegarem no documento.

image::http://composer.telemidia.puc-rio.br/_media/outline-view.png[title="{outline_view}",width="35%",link="http://composer.telemidia.puc-rio.br/_media/outline-view.png"]

[[sec:properties_view]]
Visão de Propriedades
^^^^^^^^^^^^^^^^^^^^^
A {properties_view} apresenta o conjunto de propriedades da entidade
selecionada. Essa entidade selecionada por ser, por exemplo, uma região na
{layout_view}, um objeto de mídia na {structural_view} etc.

image::http://composer.telemidia.puc-rio.br/_media/property-view.png[title="{properties_view}",width="35%",link="http://composer.telemidia.puc-rio.br/_media/property-view.png"]

[[sec:textual_view]]
Visão Textual
^^^^^^^^^^^^^
A {textual_view} permite aos usuários interagirem diretamente com o
código-fonte da aplicação NCL. Ela é um editor de texto avançado focado na
linguagem NCL e traz funcionalidades como: coloração sintática, sugestão de
código contextual e formatação de código.

A {textual_view} é recomendada especialmente para usuários avançados e que se
sentem à vontade com a sintaxe XML.

image::http://composer.telemidia.puc-rio.br/_media/textual-view.png[title="{textual_view}",width="35%",link="http://composer.telemidia.puc-rio.br/_media//textual-view.png"]

[[sec:validator_view]]
Validador
^^^^^^^^^
O {validator_view} é responsável por notificar erros e alertas no documento
NCL. Os erros no documento são apresentados como uma lista no
{validator_view}. A figura a seguir apresenta um exemplo.

image::http://composer.telemidia.puc-rio.br/_media/plugins/validator-plugin.png[title="{validator_view}",width="35%",link="http://composer.telemidia.puc-rio.br/_media/plugins/validator-plugin.png"]

## Conceitos Básicos ##

### Plugins ###
_Plugins_ são pedaços de software que podem ser distribuídos separadamente e
adicionar ou modificar funcionalidades de um determinado software _host_.
Uma lista completa dos _plugins_ disponíveis oficialmente no site do
{nclcomposer}, pode ser encontrada em:
http://composer.telemidia.puc-rio.br/en/plugins/[].

### Visões ###
O tipo de _plugin_ mais comum do {nclcomposer} é aquele que denominamos
_Visão_. Uma visão é um _plugin_ que possui uma representação na interface
gráfica e permite ao autor visualizar/editar uma característica específica do
documento NCL.

As principais visões do {nclcomposer} são: {default_views}.

.Perspectivas
Uma _perspectiva_, no NCL Composer, é a disposição das visões na tela. É
permitido ao usuário criar suas próprias perspectivas, escolhendo a que melhor
o ajuda a efetuar uma determinada tarefa.

## Usando a documentação e buscando ajuda ##
Esta documentação é dividida em três partes principais:

<<sec:iniciando, Iniciando>>::
Apresenta os primeiros passos que irá levá-lo a começar a utilizar o
{nclcomposer}, incluindo as instruções de instalação e configurações básicas
do ambiente.

<<sec:windows_and_menus, Interface Gráfica>>::
Apresenta uma visão geral das convenções da interface gráfica, e as principais
janelas e menus do {nclcomposer}.

<<sec:tutorial, Tutorial>>::
Contém uma série de exemplos e o passo-a-passo de como desenvolvê-los no
{nclcomposer}.