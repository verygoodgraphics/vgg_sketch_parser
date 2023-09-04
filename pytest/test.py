# -*- coding:utf_8 -*-

import pytest
import os

# 备注: 注意更新 sketch_parser_tool.exe
# 备注: 可通过调整 common.py 中的 resource_path, 选择被测试的 sketch 版本

try:
    os.mkdir('pytest/output')
except:
    pass

pytest.main(["pytest/src"])
#pytest.main(["pytest/src/object/test_path.py"])