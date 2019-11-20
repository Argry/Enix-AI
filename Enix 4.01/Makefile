all:
	g++ -ggdb -O0 -c ENiX_Semantics.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_CS.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Daemon.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_NLPStoreLogic.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Sentence.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Mode.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Poly.cpp -std=c++11 -lm
	g++ -ggdb -O0 -c ENiX_Chocky.cpp -std=c++11 -lm
	g++ -ggdb -O0 -c ENiX_NNBridge.cpp -std=c++11
	g++ -ggdb -O0 -c NLP_Daemon.cpp -std=c++11
	g++ -ggdb -O0 -c WitchCraft.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Disk.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Config.cpp -std=c++11
	g++ -ggdb -O0 -c WitchCraft-Colours.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Neural_Server.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Binary.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Memory.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_List.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Time.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_NLPTest.cpp -std=c++11
	g++ -ggdb -O0 -c CDS_Daemon.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Packet.cpp -std=c++11
	g++ -ggdb -O0 -c Amoeba_Daemon.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_WMSAPI.cpp -std=c++11
	g++ -ggdb -O0 -c Amoeba_CLITest.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Amoeba.cpp -std=c++11 -lpthread
	g++ -ggdb -O0 -c ENiX_IPC.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_SHM.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_TestCLINeural.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Net.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_TestClient.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_TestServer.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Awareness.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Subnet_Perm.cpp -std=c++11 -lpthread
	g++ -ggdb -O0 -c ENiX_Seq.cpp -std=c++11 -lm
	g++ -ggdb -O0 -c ENiX_Lean.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Large.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Deps.cpp -std=c++11 -lm
	g++ -ggdb -O0 -c ENiX_CORE.cpp -std=c++11
	g++ -ggdb -O0 WitchCraft-Colours.o -o TerminalColourTest.bin -std=c++11
	g++ -ggdb -O0 ENiX_Net.o ENiX_IPC.o ENiX_SHM.o ENiX_Seq.o ENiX_Packet.o WitchCraft.o ENiX_Config.o ENiX_Disk.o ENiX_Binary.o -o WitchCraft.bin -std=c++11 -lreadline
	g++ -ggdb -O0 ENiX_Net.o ENiX_List.o ENiX_Packet.o ENiX_TestCLINeural.o ENiX_Awareness.o ENiX_Seq.o ENiX_Deps.o ENiX_Subnet_Perm.o ENiX_Lean.o ENiX_IPC.o ENiX_SHM.o ENiX_Large.o ENiX_CORE.o ENiX_Binary.o -o ENiX4NeuralCLI.bin -std=c++11 -lm -lpthread
	g++ -ggdb -O0 ENiX_Disk.o ENiX_Binary.o ENiX_List.o ENiX_Amoeba.o ENiX_WMSAPI.o ENiX_Packet.o ENiX_TestClient.o ENiX_Net.o ENiX_SHM.o ENiX_Seq.o ENiX_IPC.o -o DataInjector.bin -std=c++11 -lreadline -lpthread
	g++ -ggdb -O0 ENiX_Binary.o ENiX_Packet.o ENiX_Net.o ENiX_TestServer.o ENiX_SHM.o ENiX_IPC.o -o NNTestServer.bin -std=c++11
	g++ -ggdb -O0 ENiX_Daemon.o ENiX_Subnet_Perm.o ENiX_List.o ENiX_Packet.o ENiX_Awareness.o ENiX_Lean.o ENiX_Large.o ENiX_Deps.o ENiX_CORE.o ENiX_Seq.o ENiX_Binary.o ENiX_Net.o ENiX_Neural_Server.o ENiX_SHM.o ENiX_IPC.o -o NeuralServer.bin -std=c++11 -lm -lpthread 
	g++ -ggdb -O0 Amoeba_CLITest.o ENiX_List.o ENiX_Amoeba.o ENiX_SHM.o ENiX_WMSAPI.o ENiX_Binary.o ENiX_Packet.o -o ENiX4AmoebaCLI.bin -std=c++11 -lpthread
	g++ -ggdb -O0 ENiX_Daemon.o Amoeba_Daemon.o ENiX_List.o ENiX_Binary.o ENiX_Packet.o ENiX_Net.o ENiX_IPC.o ENiX_SHM.o ENiX_Seq.o ENiX_Amoeba.o ENiX_WMSAPI.o -o AmoebaServer.bin -std=c++11 -pthread
	g++ -ggdb -O0 ENiX_Daemon.o CDS_Daemon.o ENiX_List.o ENiX_Binary.o ENiX_Packet.o ENiX_Net.o ENiX_IPC.o ENiX_SHM.o ENiX_Seq.o ENiX_Amoeba.o ENiX_WMSAPI.o -o ConnectionDistributionService.bin -std=c++11 -pthread
	g++ -ggdb -O0 ENiX_Semantics.o ENiX_NLPStoreLogic.o ENiX_Sentence.o ENiX_Mode.o ENiX_Poly.o ENiX_Seq.o ENiX_NNBridge.o ENiX_Memory.o ENiX_List.o ENiX_Time.o ENiX_Chocky.o ENiX_NLPTest.o ENiX_Packet.o ENiX_Binary.o ENiX_SHM.o ENiX_IPC.o ENiX_Net.o ENiX_WMSAPI.o ENiX_Amoeba.o ENiX_CS.o -o CLINLPTest.bin -std=c++11 -pthread
	g++ -ggdb -O0 ENiX_Semantics.o ENiX_NLPStoreLogic.o ENiX_Daemon.o ENiX_Sentence.o ENiX_Mode.o ENiX_Poly.o ENiX_NNBridge.o NLP_Daemon.o ENiX_Binary.o ENiX_Packet.o ENiX_Net.o ENiX_IPC.o ENiX_SHM.o ENiX_Seq.o ENiX_List.o ENiX_Chocky.o ENiX_Memory.o ENiX_Time.o ENiX_WMSAPI.o ENiX_Amoeba.o ENiX_CS.o -o NLPServer.bin -std=c++11 -pthread -lm

clean:
	rm -f *.o
	rm -f *.bin
	rm -f core
	rm -f *~
	rm -f *.bak
	rm -rf AmoebaTest
	rm -rf NLPTest
	rm -rf ANNTest
	rm -rf CDSTest
	rm -rf WitchCraftTest

witchcraft:
	g++ -ggdb -O0 -c WitchCraft.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Net.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_IPC.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_SHM.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Seq.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Packet.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Config.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Disk.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Binary.cpp -std=c++11
	g++ -ggdb -O0 ENiX_Net.o ENiX_IPC.o ENiX_SHM.o ENiX_Seq.o ENiX_Packet.o WitchCraft.o ENiX_Config.o ENiX_Disk.o ENiX_Binary.o -o WitchCraft.bin -std=c++11 -lreadline

nlp:
	g++ -ggdb -O0 -c ENiX_Semantics.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_CS.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Daemon.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_NLPStoreLogic.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Sentence.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Mode.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Poly.cpp -std=c++11 -lm
	g++ -ggdb -O0 -c ENiX_Chocky.cpp -std=c++11 -lm
	g++ -ggdb -O0 -c ENiX_NNBridge.cpp -std=c++11
	g++ -ggdb -O0 -c NLP_Daemon.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Binary.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Packet.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Net.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_List.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Memory.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Time.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_IPC.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_SHM.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Seq.cpp -std=c++11 -lm
	g++ -ggdb -O0 -c ENiX_WMSAPI.cpp -std=c++11
	g++ -ggdb -O0 -c ENiX_Amoeba.cpp -std=c++11
	g++ -ggdb -O0 ENiX_Semantics.o ENiX_NLPStoreLogic.o ENiX_Daemon.o ENiX_Sentence.o ENiX_Mode.o ENiX_Poly.o ENiX_NNBridge.o NLP_Daemon.o ENiX_Binary.o ENiX_Packet.o ENiX_Net.o ENiX_IPC.o ENiX_SHM.o ENiX_Seq.o ENiX_List.o ENiX_Chocky.o ENiX_Memory.o ENiX_Time.o ENiX_WMSAPI.o ENiX_Amoeba.o ENiX_CS.o -o NLPServer.bin -std=c++11 -pthread -lm

test:
	mkdir -p AmoebaTest
	mkdir -p NLPTest
	mkdir -p ANNTest
	mkdir -p CDSTest
	mkdir -p WitchCraftTest
	cp *.h AmoebaTest
	cp *.cpp AmoebaTest
	cp *.bin AmoebaTest
	cp *.h NLPTest
	cp *.cpp NLPTest
	cp *.bin NLPTest
	cp *.h ANNTest
	cp *.cpp ANNTest
	cp *.bin ANNTest
	cp *.h CDSTest
	cp *.cpp CDSTest
	cp *.bin CDSTest
	cp *.h WitchCraftTest
	cp *.cpp WitchCraftTest
	cp *.bin WitchCraftTest
