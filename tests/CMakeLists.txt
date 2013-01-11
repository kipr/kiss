ADD_EXECUTABLE(output_widget_test output_widget_test.cpp)
TARGET_LINK_LIBRARIES(output_widget_test kisside pcompiler kar ${QT_LIBRARIES})

# ADD_EXECUTABLE(project_model_test project_model_test.cpp)
# TARGET_LINK_LIBRARIES(project_model_test ${QT_LIBRARIES} kisside pcompiler kar)

ADD_EXECUTABLE(template_test template_test.cpp)
TARGET_LINK_LIBRARIES(template_test kisside pcompiler kar ${QT_LIBRARIES})

ADD_EXECUTABLE(save_as_dialog_test save_as_dialog_test.cpp)
TARGET_LINK_LIBRARIES(save_as_dialog_test kisside pcompiler kar ${QT_LIBRARIES})
