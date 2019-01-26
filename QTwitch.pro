TEMPLATE = subdirs

SUBDIRS = \
    Api \
    Models \
    tests

analyze.target = pvs_studio
analyze.CONFIG = recursive
analyze.recurse = Api Models
QMAKE_EXTRA_TARGETS = analyze
