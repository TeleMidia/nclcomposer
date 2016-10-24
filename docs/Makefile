# Makefile for NCL Composer Tutorial
#
# Dependencies:
#   * mkdocs

all:
	ls -r relnotes/* | xargs cat  >pt-br/relnotes.md
	mkdocs build --clean
	#mkdir -p ${BUILD_DIR}/${LANG}
	#$(foreach i,$(INPUT), lua ${LANG}/dict.lua $(i) >${BUILD_DIR}/$(i); )

