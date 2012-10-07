ADD_EXECUTABLE(output_widget_test output_widget_test.cpp)
TARGET_LINK_LIBRARIES(output_widget_test ${QT_LIBRARIES} kisside pcompiler)

ADD_EXECUTABLE(code_editor_test code_editor_test.cpp)
TARGET_LINK_LIBRARIES(code_editor_test ${QT_LIBRARIES} kisside pcompiler)

ADD_EXECUTABLE(project_model_test project_model_test.cpp)
TARGET_LINK_LIBRARIES(project_model_test ${QT_LIBRARIES} kisside pcompiler)

ADD_EXECUTABLE(template_test template_test.cpp)
TARGET_LINK_LIBRARIES(template_test ${QT_LIBRARIES} kisside pcompiler)