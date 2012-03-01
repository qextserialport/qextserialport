OTHER_FILES += $$PWD/qextserialport.qdocconf

docs_target.target = docs
docs_target.commands = qdoc3 $$PWD/qextserialport.qdocconf

QMAKE_EXTRA_TARGETS = docs_target
QMAKE_CLEAN += "-r $$PWD/html"

