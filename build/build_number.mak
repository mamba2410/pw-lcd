
##########################################################
# Recipes and variables to keep track of build numbers
##########################################################

# Get the build number file, get the actual number, and get the date
BUILD_NUMBER_FILE = ./build/build_number
BUILD_DATE = $$(date +'%Y%m%d')
BUILD_NUMBER = $$(cat $(BUILD_NUMBER_FILE))	

# Recipe to update the build number file
# .PHONY to make it always run
.PHONY : build_number
build_number: 
	@[ -f $(BUILD_NUMBER_FILE) ] || echo 0 > $(BUILD_NUMBER_FILE)
	@echo $$(( $$(cat $(BUILD_NUMBER_FILE)) + 1)) > $(BUILD_NUMBER_FILE)
	@echo "Build date: $(BUILD_DATE)"
	@echo "Build number: $(BUILD_NUMBER)"




