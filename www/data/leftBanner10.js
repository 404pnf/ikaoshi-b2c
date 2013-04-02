//2——普通用户; 21——EGS管理员; 22——EGS编辑

var root_menu = new Array()
root_menu = {
	"2":{
		"-1":{"title":"英语水平定位","link":"javascript:listPaper(\'exam_structure_list\',10,{other_status:false});"},
		
		"0":{"title":"专项训练",
			"layer2":{
				"0":{"title":"四级专项训练","link":"javascript:specialModuleList(\'四级专项训练\');"},
				"1":{"title":"六级专项训练","link":"javascript:specialModuleList(\'六级专项训练\');"},
				"2":{"title":"专四专项训练","link":"javascript:specialModuleList(\'专四专项训练\');"},
				"3":{"title":"专八专项训练","link":"javascript:specialModuleList(\'专八专项训练\');"},
				"4":{"title":"PETS1专项训练","link":"javascript:specialModuleList(\'PETS1专项训练\');"},
				"5":{"title":"PETS2专项训练","link":"javascript:specialModuleList(\'PETS2专项训练\');"},
				"6":{"title":"PETS3专项训练","link":"javascript:specialModuleList(\'PETS3专项训练\');"},
				"7":{"title":"PETS4专项训练","link":"javascript:specialModuleList(\'PETS4专项训练\');"},
				"8":{"title":"PETS5专项训练","link":"javascript:specialModuleList(\'PETS5专项训练\');"},
				"9":{"title":"PRETCO-A专项训练","link":"javascript:specialModuleList(\'PRETCO-A专项训练\');"},
				"10":{"title":"PRETCO-B专项训练","link":"javascript:specialModuleList(\'PRETCO-B专项训练\');"}
				}
			},
		"1":{"title":"模考测试",
			"layer2":{
				"0":{"title":"考研英语","link":"javascript:listPaper(\'exam_structure_list\',29,{other_status:true,other_op:\'paper_info_by_term_id\',other_tid:28});"},
				"1":{"title":"大学英语四级","link":"javascript:listPaper(\'exam_structure_list\',19,{other_status:true,other_op:\'paper_info_by_term_id\',other_tid:1});"},
				"2":{"title":"大学英语六级","link":"javascript:listPaper(\'exam_structure_list\',20,{other_status:true,other_op:\'paper_info_by_term_id\',other_tid:4});"},
				"3":{"title":"英语专业四级","link":"javascript:listPaper(\'exam_structure_list\',18,{other_status:true,other_op:\'paper_info_by_term_id\',other_tid:21});"},
				"4":{"title":"英语专业八级","link":"javascript:listPaper(\'exam_structure_list\',23,{other_status:true,other_op:\'paper_info_by_term_id\',other_tid:22});"},
				
				"5":{"title":"全国英语等级考试","link":"javascript:listPaper(\'exam_structure_list\',24,{other_status:false,other_op:\'paper_info_by_term_id\',other_tid:4});"},
				"6":{"title":"高校英语应用能力考试","link":"javascript:listPaper(\'exam_structure_list\',26,{other_status:true,other_op:\'paper_info_by_term_id\',other_tid:25});"}
				
				}
			},
		"2":{"title":"我的历史记录","link":"loadurl(\'user_exam.html\');"}
	},
	"21":{
		"0":{"title":"题库管理",
			"layer2":{
				"0":{"title":"出题","link":"loadurl(\'add_item.html\');"},
				"1":{"title":"试题查询","link":"loadurl(\'manage_item.html\');"},
				"2":{"title":"基本题型管理","link":"javascript:showTagsManage(\'#mainContent\',\'basic\');"},
				"3":{"title":"题型管理","link":"javascript:showTagsManage(\'#mainContent\',\'item_type\');"},
				"4":{"title":"标签管理","link":"javascript:showTagsManage(\'#mainContent\',\'tags\');"},
				"5":{"title":"考点管理","link":"javascript:showTagsManage(\'#mainContent\',\'attribution_info\');"}
				}
			},
		"1":{"title":"模板与试卷",
			"layer2":{
				"0":{"title":"建立模板","link":"javascript:loadurl(\'zujuanmodule.html\');"},
				"1":{"title":"模板列表","link":"javascript:loadurl(\'manage_modules.html\');"},
				"2":{"title":"试卷管理","link":"javascript:managePaper(\'paper_info_by_term_id\',0);"},
				"3":{"title":"试卷类型","link":"javascript:showTagsManage(\'#mainContent\',\'exam\');"}			
				}
			},
		"2":{"title":"设置",
			"layer2":{
				"0":{"title":"试卷评语资源", "link":"loadurl(\'settings\/exam_comments_resources.html\');"}
				}
			},
		"3":{"title":"导入与统计",
			"layer2":{
				"0":{"title":"用户历史记录", "link":"loadurl(\'user_exam.html\');"},
				"1":{"title":"试卷导入","link":"loadurl(\'delete_import_by_liushui.html\');"},
				"2":{"title":"做题统计","link":"loadurl(\'tongji.html\');"}
				}
			}
	},
	"22":{
		"0":{"title":"题库管理",
			"layer2":{
				"0":{"title":"出题","link":"loadurl(\'add_item.html\');"},
				"1":{"title":"查询管理","link":"loadurl(\'manage_item.html\');"},
				"2":{"title":"标签管理","link":"javascript:showTagsManage(\'#mainContent\',\'tags\');"},
				"3":{"title":"题型管理","link":"javascript:showTagsManage(\'#mainContent\',\'item_type\');"},
				"4":{"title":"基本题型管理","link":"javascript:showTagsManage(\'#mainContent\',\'basic\');"}
				}
			},
		"1":{"title":"模板与试卷",
			"layer2":{
				"0":{"title":"建立模板","link":"javascript:loadurl(\'zujuanmodule.html\');"},
				"1":{"title":"模板列表","link":"javascript:listPaper(\'exam_structure_list\');"},
				"2":{"title":"评语管理","link":"javascript:showTagsManage(\'#mainContent\',\'comments\');"},
				"3":{"title":"考点评语管理","link":"javascript:showTagsManage(\'#mainContent\',\'attr_comments\');"},
				"4":{"title":"试卷管理","link":"javascript:listPaper(\'paper_info_by_term_id\',0);"},
				"5":{"title":"试卷类型","link":"javascript:showTagsManage(\'#mainContent\',\'exam\');"}			
				}
			},
		"2":{"title":"我的历史记录","link":"loadurl(\'user_exam.html\');"}
	}
};