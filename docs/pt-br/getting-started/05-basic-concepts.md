# Conceitos Básicos

A linguagem NCL---e consequentemente, o NCL Composer---baseiam-se no modelo NCM
(Nested Context Model) para modelar aplicações multimídia interativas.  No NCM,
aplicações multimídia são modeladas por meio de nós (__nodes__) e elos
(__links__) (Figura 1).

Os __nós__ representam os objetos de mídias da aplicação (vídeos, imagens,
textos etc.), enquanto os __elos__ representam relacionamentos entres esses
nós.

![](../img/05-basic-concepts-nodes-and-links.png)

Além dos nós de mídias, o NCM também permite agrupar nós e links em __nós de
composição__ (ou __contextos__), que permitem agrupar logicamente os nós e elos
de uma aplicação.  Contextos podem também agrupar outros contextos, recursivamente.

![](../img/05-basic-concepts-context.png)

Modelar uma aplicação multimídia no NCM então é definir __quais__ os objetos de
mídia fazem parte da aplicação (através dos __nós de mídia__), __como__ eles
estão estruturados (definindo-se __nós de contextos__) e __quando__ esses objetos
de mídia serão apresentados (o que é realizado por meio da definição de __elos__
entre os nós de mídia).

# O software NCL Composer

## Visões
Uma visão traz alguma representação (geralmente gráfica) que permite
visualizar/editar uma característica específica do documento NCL.

As principais visões do {nclcomposer} são: {default_views}.

## Perspectivas
Uma _perspectiva_, no {nclcomposer}, é a disposição das visões na tela. É
permitido ao usuário criar suas próprias perspectivas, escolhendo a que melhor
o ajuda a efetuar uma determinada tarefa.

