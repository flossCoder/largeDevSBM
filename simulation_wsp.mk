.PHONY: clean All

All:
	@echo ----------Building project:[ simulation - Release ]----------
	@"$(MAKE)" -f "simulation.mk"
clean:
	@echo ----------Cleaning project:[ simulation - Release ]----------
	@"$(MAKE)" -f "simulation.mk" clean
