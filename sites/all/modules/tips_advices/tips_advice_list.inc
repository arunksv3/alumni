<?php
/**
 * @file
 * A tips and advice module that displays user information.
 */
function tips_advice_list($form, &$form_state){              
    $nid = 2;
    $argvalue = "";
    $cats = cat_tips_advice();    
    $general_advice_list = general_advice_list();
    $nature_businesss_arr = nature_bussiness_arr();
    $fields_of_study_arr = fields_of_study();
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    if(arg(1)!="")
        $argvalue = (int)decrypt(arg(1),$secret_key);
    
    /* basic page text */
    $node = node_load($nid);
    if($node!=""){
        $nodeView = node_view($node, 'full');        
        $message = drupal_render($nodeView['body']);
        $message .= drupal_render($nodeView['field_pageimage']);
        $form['cutomtext'] = array(
            '#type' => 'item',
            '#markup' => $message,
        );  
    }
    
    /* query to fetch tips & advices based on categories */
    $tips_advices_list = db_select('tips_advices');
    $tips_advices_list->fields('tips_advices');
    $tips_advices_list->condition('tips_status', 1, '=');
    $tips_advices_list->condition('delete_status', '0', '='); 
    
    if(isset($form_state['input']['faculty']) && $form_state['input']['faculty']!=""){
        $userid = $form_state['input']['faculty'];
        $tips_advices_list->condition('uid', $userid, '=');
    }else{
        if($argvalue==""){        
            $tips_advices_list->condition('category', 0, '=');
        }else{        
            $tips_advices_list->condition('category', $argvalue, '=');
        }  
    }
    
    if($argvalue=="" || $argvalue=="0"){ 
        if (isset($form_state['input']['general_advice']) && $form_state['input']['general_advice']!="") {            
            $general_advice = $form_state['input']['general_advice']; 
            $tips_advices_list->condition('general_advice', $general_advice, '=');
        }
    }else if ($argvalue=="1") {
        if (isset($form_state['input']['career_p']) && $form_state['input']['career_p']!="") {
            $specializations = $form_state['input']['career_p'];
            $tips_advices_list->condition('career_p', trim($specializations), '=');
        }
        if (isset($form_state['input']['career_sp']) && $form_state['input']['career_sp']!="") {
            $career_sp = $form_state['input']['career_sp']; 
            $tips_advices_list->condition('career_sp', trim($career_sp), '=');
        } 
        if(isset($form_state['input']['career_c']) && $form_state['input']['career_c']!=""){
            $career_c = $form_state['input']['career_c'];
            $tips_advices_list->condition('career_c', trim($career_c), '=');  
        }   
    }else if ($argvalue=="2") {
        if (isset($form_state['input']['field_of_study']) && $form_state['input']['field_of_study']!="") {
            $field_of_study = $form_state['input']['field_of_study'];
            $tips_advices_list->condition('field_of_study', trim($field_of_study), '=');
            if(isset($form_state['input']['qualification']) && $form_state['input']['qualification']!=""){
                $qualification = $form_state['input']['qualification'];
                $tips_advices_list->condition('qualification', trim($qualification), '=');  
            }
            if(isset($form_state['input']['qualification_level']) && $form_state['input']['qualification_level']!=""){
                $qualification_level = $form_state['input']['qualification_level'];
                $tips_advices_list->condition('qualification_level', trim($qualification_level), '=');  
            }
        }
    }else if ($argvalue=="3") {
        if (isset($form_state['input']['advice_industries']) && $form_state['input']['advice_industries']!="") {
            $advice_industries = $form_state['input']['advice_industries'];
            $tips_advices_list->condition('advice_industries', trim($advice_industries), '=');  
        }
    }else{
        if (isset($form_state['input']['faculty']) && $form_state['input']['faculty']!="") {
            $faculty = $form_state['input']['faculty'];
            $tips_advices_list->condition('uid', trim($faculty), '=');  
        }
    }
    
    $query = $tips_advices_list->execute();
    $result = $query->fetchAll(); 
       
    $menulistArr = $cats + array(count($cats)=>'Faculty');     
    $customlinks = '<ul>';
    foreach ($menulistArr as $key => $value) {         
        $customlinks .=  '<li>'.l(t($value), 'tipsandadvice/'.encrypt($key,$secret_key)).'</li>';        
    }
    $customlinks .= '<ul>';
    
    $form['customlinks'] = array(
        '#type' => 'item',
        '#markup' => $customlinks,
    );
    
    $form['argvalue'] = array(
        '#type' => 'hidden',
        '#value' => $argvalue,
    );
    
    if(isset($form_state['input']['argvalue']))
        $argvalue = $form_state['input']['argvalue'];
    
    $form['searchheader'] = array(
        '#type' => 'item',
        '#prefix' => '<div class="searchbox">',        
    );    
    
    /* Search based on selected categories */
    if ($argvalue == "" || $argvalue == "0") {
        // Province dropdown list
        $form['general_advice'] = array(
            '#type' => 'select',
            '#title' => t('General Advice'),
            '#options' => $general_advice_list,
            '#default_value' => (isset($form_state['input']['general_advice']) ? $form_state['input']['general_advice'] : ''),
        );
    }else if ($argvalue == "1") {
        $category_parrents = db_select('specializations_p')
                    ->fields('specializations_p')
                    ->execute()
                    ->fetchAll();
        $display_category_parrents[''] = 'All Specializations';
        foreach ($category_parrents as $category_parrent_name):
            $display_category_parrents[$category_parrent_name->sid] = $category_parrent_name->name;
        endforeach;
        $form['wrapper']['career_p'] = array(
            '#type' => 'select',
            '#title' => t('Specialization'),
            '#options' => $display_category_parrents,
            '#ajax' => array(
                'event' => 'change',
                'callback' => 'tips_advice_subparrent_list_ajax',
                'wrapper' => 'tips-advice-subparrent-list-wrapper',
            ),
            '#default_value' => (isset($form_state['input']['career_p']) ? $form_state['input']['career_p'] : ''),
        );

        // Wrapper for sub parent dropdown list
        $form['wrapper']['wrapper_parent'] = array(
            '#prefix' => '<div id="tips-advice-subparrent-list-wrapper">',
            '#suffix' => '</div>',
        );

        if (isset($form_state['input']['career_p']) && $form_state['input']['career_p'] != "") {
            $category_subparrents = db_select('specializations_sp')
                    ->fields('specializations_sp')
                    ->condition('spid', $form_state['input']['career_p'], '=')
                    ->execute()
                    ->fetchAll();
            $display_category_subparrents[''] = 'All';
            foreach ($category_subparrents as $category_subparrent_name):
                $display_category_subparrents[$category_subparrent_name->sid] = $category_subparrent_name->name;
            endforeach;         
            // Province dropdown list
            $form['wrapper']['wrapper_parent']['career_sp'] = array(
                '#type' => 'select',
                '#options' => $display_category_subparrents,
                '#ajax' => array(
                    'event' => 'change',
                    'callback' => 'tips_advice_child_list_ajax',
                    'wrapper' => 'child-list-wrapper',
                ),
                '#default_value' => (isset($form_state['input']['career_sp']) ? $form_state['input']['career_sp'] : ''),
            );
        }

        // Wrapper for child dropdown list
        $form['wrapper']['wrapper_parent']['wrapper_child'] = array(
            '#prefix' => '<div id="child-list-wrapper">',
            '#suffix' => '</div>',
        );

        if (isset($form_state['input']['career_sp']) && $form_state['input']['career_sp'] != "") {
            $category_childs = db_select('specializations_c')
                    ->fields('specializations_c')
                    ->condition('cid', $form_state['input']['career_sp'], '=')
                    ->execute()
                    ->fetchAll();
            $display_category_childs[''] = 'All';
            foreach ($category_childs as $category_child_name):
                $display_category_childs[$category_child_name->sid] = $category_child_name->name;
            endforeach;
            // Province dropdown list
            $form['wrapper']['wrapper_parent']['wrapper_child']['career_c'] = array(
                '#type' => 'select',
                '#options' => $display_category_childs,
                '#default_value' => (isset($form_state['input']['career_c']) ? $form_state['input']['career_c'] : ''),
            );
        }
    }else if ($argvalue == "3") {
        // Province dropdown list
        $form['wrapper']['advice_industries'] = array(
            '#type' => 'select',
            '#title' => t('Advice by Industries'),
            '#options' => array('0'=>'Select') + $nature_businesss_arr,
            '#default_value' => (isset($form_state['input']['advice_industries']) ? $form_state['input']['advice_industries'] : ''),
        );
    }else if ($argvalue=="2") {
        $form['wrapper']['field_of_study'] = array(
            '#type' => 'select',
            '#title' => t('Field of Study'),
            '#options' => array(''=>'Select') + $fields_of_study_arr,
            '#ajax' => array(
                'event' => 'change',
                'callback' => 'tips_advice_fields_of_study_list_ajax',
                'wrapper' => 'tips-advice-field-of-study-list-wrapper',
            ),
            '#default_value' => (isset($form_state['input']['field_of_study']) ? $form_state['input']['field_of_study'] : ''),
        );

        // Wrapper for sub parent dropdown list
        $form['wrapper']['wrapper_field_of_study'] = array(
            '#prefix' => '<div id="tips-advice-field-of-study-list-wrapper">',
            '#suffix' => '</div>',
        );

        if (isset($form_state['input']['field_of_study']) && $form_state['input']['field_of_study'] != "") {   
            $qualification_list = array(''=>'Select') + qualification_list();
            // Province dropdown list
            $form['wrapper']['wrapper_field_of_study']['qualification'] = array(
                '#type' => 'select',
                '#title' => t('Qualification'),
                '#options' => $qualification_list,
                '#ajax' => array(
                    'event' => 'change',
                    'callback' => 'tips_advice_qualification_list_ajax',
                    'wrapper' => 'qualification-list-wrapper',
                ),
                '#default_value' => (isset($form_state['input']['qualification']) ? $form_state['input']['qualification'] : ''),
            );
        }

        // Wrapper for child dropdown list
        $form['wrapper']['wrapper_field_of_study']['wrapper_qualification'] = array(
            '#prefix' => '<div id="qualification-list-wrapper">',
            '#suffix' => '</div>',
        );

        if (isset($form_state['input']['qualification']) && $form_state['input']['qualification'] != "") {  
            $qid = $form_state['input']['qualification'];                
            $qualification_level_arr = array(''=>'Select') + highest_qualification_list($qid);
            // Province dropdown list
            $form['wrapper']['wrapper_field_of_study']['wrapper_qualification']['qualification_level'] = array(
                '#type' => 'select',
                '#title' => t('Qualification Level'),
                '#options' => $qualification_level_arr,
                '#default_value' => (isset($form_state['input']['qualification_level']) ? $form_state['input']['qualification_level'] : ''),
            );
        }
    }else{
        $ldap_user_list = db_select('ldap_user_info','ldp');
        $ldap_user_list->join('tips_advices', 'tips', 'tips.uid = ldp.uid');            
        $ldap_user_list->fields('ldp', array('uid','givenname'));        
        $ldap_user_list->condition('ldp.orcldefaultprofilegroup', '%'. db_like('iium_staff') . '%', 'LIKE');
        $query = $ldap_user_list->execute();
        $faculty_list = $query->fetchAll(); 
        $faculty_Arr = array();

        foreach ($faculty_list as $value) {
            $faculty_Arr[$value->uid] = $value->givenname;
        }
        
        // ldap users dropdown list
        $form['faculty'] = array(
            '#type' => 'select',
            '#title' => t('Faculty'),
            '#options' => array('Select') + $faculty_Arr,
            '#default_value' => (isset($form_state['input']['faculty']) ? $form_state['input']['faculty'] : ''),
        );
    }
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Search'),   
        '#submit' => array('tips_advices_list_submit'),
    );
    
    $form['searchfooter'] = array(
        '#type' => 'item',
        '#prefix' => '</div>',        
    );
    
    // count of result
    $count = count($result); 
    
    $output = "";
    if($count > 0){
        // pagination script starts    
        $per_page = 10;
        // Initialize the pager
        $current_page = pager_default_initialize($count, $per_page);
        // Split your list into page sized chunks
        $chunks = array_chunk($result, $per_page, TRUE);
        
        $output .= '<ul class="tips-list">';
       
        $i = 0;
        foreach ($chunks[$current_page] as $value) {
            //Image variables
            $image_path = array(
                'path' => $value->image_path, 
                'alt' => $value->image_name,
                'title' => $value->image_name,  
                'width' => '150',
                'height' => '150', 
            );
            
            if($value->category=="0"){
                $category =  $general_advice_list[$value->general_advice];
            }else if($value->category=="1"){
                $category =  getSpecializationParentName($value->career_p).($value->career_sp!="0"&&$value->career_sp!=""?' >> '.getSpecializationSubparent($value->career_sp):'').($value->career_c!="0"&&$value->career_c!=""?' >> '.getSpecializationChild($value->career_c):'');
            }else if($value->category=="2"){
                $category =  ($value->field_of_study!="0"&&$value->field_of_study!=""?$fields_of_study_arr[$value->field_of_study]:'').($value->qualification!="0"&&$value->qualification!=""?' >> '.get_qualiafication_name($value->qualification):'').($value->qualification_level!="0"&&$value->qualification_level!=""?' >> '.get_qualiafication_level_name($value->qualification_level):'');
            }else{
                $category =  $nature_businesss_arr[$value->advice_industries];
            }
            
            if($i%2=='0')
                $class = "grey";
            else
                $class = "white";
            
            $output .= '<li class='.$class.'>
                            <div class="tips-left">'.theme('image', $image_path).'</div>
                            <div class="tips-right">
                                <h1>'.l($value->title, 'tipsandadvice/view_tip/'.encrypt($value->id,$secret_key)).'</h1>
                                <h2><span>Category: </span>'.$cats[$value->category].($category!=""&&$category!="0"? ' >> '.$category:'').'</h2>
                                <h3><span>Posted by: </span>'.$value->author_name.'</h3>                                    
                                <p>'.limit_words($value->content_desc,20).'</p>                                   
                                '.l('Read More', 'tipsandadvice/view_tip/'.encrypt($value->id,$secret_key)).'
                            </div>
                        </li>'; 
            $i++;
        }
        
        
        
        $output .= '</ul>';
        
        // Show the pager
        $output .= theme('pager', array('quantity', $count)); 
    }else{
        $output = 'No Tips and Advices found on this category, please try it later.'; 
    }
    
    $form['output'] = array(
        '#type' => 'item',
        '#markup' => $output,
    );
    
    return $form;
}

/**
 * Tips & Advice search submission
 */
function tips_advices_list_submit($form, &$form_state) {      
    $form_state['rebuild'] = TRUE;
}
