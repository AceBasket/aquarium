SUBDIRS = controller view

.PHONY: all clean test $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

test:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir test; \
	done

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done