# -*- coding:utf_8 -*-

import pytest
import os

# 备注: 注意更新 sketch_parser_tool.exe

try:
    os.mkdir('pytest/output')
except:
    pass

pytest.main(["pytest/src"])
#pytest.main(["pytest/src/object/test_basic_attr.py"])