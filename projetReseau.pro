######################################################################
# Automatically generated by qmake (2.01a) dim. janv. 5 12:15:35 2014
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . \
              client \
              client2 \
              partition \
              Serveur \
              structure \
              client/interface \
              client2/interface \
              Serveur/clientData \
              Serveur/reseau \
              Serveur/threadData \
              structure/clientData
INCLUDEPATH += . \
               client \
               structure \
               structure/clientData \
               client/interface \
               Serveur/reseau \
               Serveur/threadData \
               Serveur \
               client2/interface \
               client2 \
               Serveur/clientData

# Input
HEADERS += client/client.h \
           client/fichiers.h \
           client/sock.h \
           client/sockdist.h \
           client2/client.h \
           client2/fichiers.h \
           client2/sock.h \
           client2/sockdist.h \
           Serveur/Serveur.h \
           structure/structure.h \
           client/interface/interface.h \
           client2/interface/interface.h \
           Serveur/clientData/DonneeClient.h \
           Serveur/clientData/TableauClient.h \
           Serveur/reseau/sock.h \
           Serveur/reseau/sockdist.h \
           Serveur/threadData/DonneeThread.h \
           Serveur/threadData/TableauThread.h \
           structure/clientData/DonneeClient.h \
           structure/clientData/TableauClient.h
SOURCES += client/client.cc \
           client/fichiers.cc \
           client/main.cc \
           client/sock.cc \
           client/sockdist.cc \
           client/teste.cc \
           client2/client.cc \
           client2/fichiers.cc \
           client2/main.cc \
           client2/sock.cc \
           client2/sockdist.cc \
           client2/teste.cc \
           partition/main.cc \
           Serveur/principal.cc \
           Serveur/Serveur.cc \
           client/interface/interface.cc \
           client/interface/testeQt.cc \
           client2/interface/interface.cc \
           client2/interface/testeQt.cc \
           Serveur/clientData/DonneeClient.cc \
           Serveur/clientData/TableauClient.cc \
           Serveur/reseau/sock.cc \
           Serveur/reseau/sockdist.cc \
           Serveur/threadData/DonneeThread.cc \
           Serveur/threadData/TableauThread.cc \
           structure/clientData/DonneeClient.cc \
           structure/clientData/TableauClient.cc
