all:
	@$(foreach dir, $(wildcard */), cd $(dir) && $(MAKE) all -j 8 ARCH="$(ARCH)" CC="$(CC)" CXX="$(CXX)" LD="$(LD)" BUILD="../../build/" && cd ..;)
	@$(foreach dir, $(wildcard */), cd $(dir) && cd ..;)

clean:
	@$(foreach dir, $(wildcard */), cd $(dir) && $(MAKE) clean && cd ..;)