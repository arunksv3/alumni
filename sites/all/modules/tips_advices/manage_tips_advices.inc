<?php 
/**
 * @file
 * A tips and advice module that displays tips and advices.
 */
function manage_tips_advices($form, &$form_state){     
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');

     /* inline javascript */
    drupal_add_js(' 
        function del(){
            var cnf = confirm("Are you sure you want to delete this Tip? please click Ok to delete.");
            if(cnf)
                return true;
            else
                return false;    
        }
        jQuery(document).ready(function($) {
            $(".popup").click(function () {                    
                window.open($(this).attr("href"), "PopupWindow", "width=600,height=600,scrollbars=yes,resizable=no");
                return false;
            });
            $("#from").datepicker({
                changeMonth: true,
                changeYear: true,
                dateFormat:"dd/mm/yy",
                showOn: "both",
                buttonImage: "'.file_create_url('public://calendar.gif').'",
                buttonImageOnly: true,
                onSelect: function(selected) {
                    $("#to").datepicker("option","minDate", selected)
                }
            });
            $("#to").datepicker({
                changeMonth: true,
                changeYear: true,
                dateFormat:"dd/mm/yy",
                showOn: "both",
                buttonImage: "'.file_create_url('public://calendar.gif').'",
                buttonImageOnly: true,
                onSelect: function(selected) {
                    $("#from").datepicker("option","maxDate", selected)
                }
            });
        });', 'inline');
    
    global $user;
    global $base_url;
    $links_admin = "";
    $cat_tips_advice = cat_tips_advice();
    $tips_status = tips_status();
    $article_type = article_type();
    $key = variable_get('IIUM_Encryption_Key', NULL);    
    if($user->uid==1 || in_array('faculty_admin', $user->roles))
        $links_admin = 'admin/config/people/'; 
    
    /* build search query */
    $query = db_select('tips_advices', 'tps');
    $query->fields('tps',array('id','tip_id','uid','title','category','article_type','created_date','tips_status','delete_status'));
    $query->condition('delete_status', '0', '=');
    if($user->uid!=1)
        $query->condition('uid', $user->uid, '=');
    
    if(!empty($form_state['input'])){        
        if(isset($form_state['input']['keyword']) && $form_state['input']['keyword']!=""){
            $keyword = $form_state['input']['keyword'];
            $query->condition('title', '%'. db_like(trim($keyword)) . '%', 'LIKE');        
        }
        if (isset($form_state['input']['category']) && $form_state['input']['category']!="") {
            $category = $form_state['input']['category'];
            $query->condition('category', trim($category), '=');
            if ($form_state['values']['category'] == "0") {
                if (isset($form_state['input']['general_advice']) && $form_state['input']['general_advice']!="") {
                    $general_advice = $form_state['input']['general_advice'];
                    $query->condition('general_advice', trim($general_advice), '=');                      
                }
            }else if($form_state['values']['category'] == "1"){
                if (isset($form_state['input']['career_p']) && $form_state['input']['career_p']!="") {
                    $specializations = $form_state['input']['career_p'];
                    $query->condition('career_p', trim($specializations), '=');
                }
                if (isset($form_state['input']['career_sp']) && $form_state['input']['career_sp']!="") {
                    $career_sp = $form_state['input']['career_sp']; 
                    $query->condition('career_sp', trim($career_sp), '=');
                } 
                if(isset($form_state['input']['career_c']) && $form_state['input']['career_c']!=""){
                    $career_c = $form_state['input']['career_c'];
                    $query->condition('career_c', trim($career_c), '=');  
                }  
            }else if($form_state['values']['category'] == "2"){
                if (isset($form_state['input']['field_of_study']) && $form_state['input']['field_of_study']!="") {
                    $field_of_study = $form_state['input']['field_of_study'];
                    $query->condition('field_of_study', trim($field_of_study), '=');
                    if(isset($form_state['input']['qualification']) && $form_state['input']['qualification']!=""){
                        $qualification = $form_state['input']['qualification'];
                        $query->condition('qualification', trim($qualification), '=');  
                    }
                    if(isset($form_state['input']['qualification_level']) && $form_state['input']['qualification_level']!=""){
                        $qualification_level = $form_state['input']['qualification_level'];
                        $query->condition('qualification_level', trim($qualification_level), '=');  
                    }
                }
            }else{
                if (isset($form_state['input']['advice_industries']) && $form_state['input']['advice_industries']!="") {
                    $advice_industries = $form_state['input']['advice_industries'];
                    $query->condition('advice_industries', trim($advice_industries), '=');  
                }
            }
        }
        if(isset($form_state['input']['article_type']) && $form_state['input']['article_type']!=""){
            $type = $form_state['input']['article_type'];
            $query->condition('article_type', '%'. db_like(trim($type)) . '%', 'LIKE');        
        }
        if( $form_state['input']['from'] && $form_state['input']['to']){            
            $from_input = explode('/', $form_state['input']['from']);
            $from_year = $from_input[2] . '-' . $from_input[0] . '-' . $from_input[1];             
            $to_input = explode('/', $form_state['input']['to']);
            $to_year = $to_input[2] . '-' . $to_input[0] . '-' . $to_input[1];             
            $query->condition('created_date', array($from_year, $to_year), 'BETWEEN');          
        }
        if(isset($form_state['input']['status']) && $form_state['input']['status']!=""){
            $status = $form_state['input']['status'];
            $query->condition('tips_status', $status, '=');        
        }
    }   
    
    $query->orderBy('id', 'DESC');
    $query = $query->execute();     
    
    // delete the record based on tip id
    if(isset($_GET) && isset($_GET['del'])){        
        $q = (int)decrypt($_GET['del'],$key);
        // query to retreive and unlink the image
        $update = db_update('tips_advices')
            ->fields(array(
                'delete_status' => '1',
            ))
            ->condition('id', $q, '=')          
            ->execute();                
        if($update){            
            drupal_set_message('Tips and Advice deleted successfully'); 
            $tips_advices_id=tips_advices_id($q);
            audit_log('Deleted Tips and Advice', "Tip ID: ".$tips_advices_id->tip_id);
            drupal_goto($links_admin.'tips_advices');
        }
    }
    
    // delete the record based on tip id
    if(isset($_GET) && isset($_GET['approve'])){        
        $q = (int)decrypt($_GET['approve'],$key);
        // query to retreive and unlink the image
        $update = db_update('tips_advices')
            ->fields(array(
                'tips_status' => '1',
            ))
            ->condition('id', $q, '=')            
            ->execute();                
        if($update){ 
            /* build search query */
            $query = db_select('tips_advices', 'tps');
            $query->fields('tps',array('id','tip_id','uid','delete_status'));
            $query->condition('delete_status', '0', '=');            
            $query->condition('id', $q, '=');            
            $resultArr = $query->execute()->fetchAssoc(); 
            
            $path = $base_url.'/'.drupal_get_path('theme', 'bootstrap_business');

            $my_module = 'tips_advice';
            $my_mail_token = 'tips_advice_waiting_approval';
            $from = variable_get('site_mail');
            $subject = "Tips & Advice Approved!!";
            $linkuserArr = user_load($resultArr['uid']);            
            
            $body = '<html xmlns="http://www.w3.org/1999/xhtml">
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title>IIUM Email</title>
            </head>
            <body style="font-family:Arial, Helvetica, sans-serif; font-size:14px; line-height:30px; margin:0; padding:0;">
                <table width="600" border="0" cellspacing="0" cellpadding="0" style="margin:0 auto; border: 1px solid #ccc; padding:0 0 15px 0;">
                    <tr>
                      <td style="background:url(' . $path . '/images/header-bg.jpg) repeat-x left top;border-bottom: 5px solid #9c2b3d; padding: 15px 10px; text-align: center;"><img src="' . $path . '/images/logo.png" width="228" height="83" /></td>
                    </tr>
                     <tr>
                        <td>Hi '.$linkuserArr->name.',</td>
                    </tr>
                    <tr>
                        <td>The Tip '.$resultArr['tip_id'].' has been reviewed and approved by Administrator, thanks for your valuable contribution.</td>
                    </tr>
                    <tr>
                        <td>Regards, </br>
                            IIUM Career Consuling Team
                        </td>
                    </tr>
                </table>    
            </body>
            </html>';

            $message = array(
                'to' => $linkuserArr->mail,
                'subject' => $subject,
                'body' => $body,
                'headers' => array(
                    'From' => $from,
                    'MIME-Version' => '1.0',
                    'Content-Type' => 'text/html;charset=utf-8'),
            );

            //Drupal mail function core
            $system = drupal_mail_system($my_module, $my_mail_token);
            if ($system->mail($message)) { 
                $tips_advices_id=tips_advices_id($q);
                audit_log('Approved Tips and Advice', "Tip ID: ".$tips_advices_id->tip_id);
                drupal_set_message('Tip Approved successfully'); 
            }
            drupal_goto($links_admin.'tips_advices');
        }
    }
    
    $result = array();       
    while ($row = $query->fetchAssoc($query)) {
        if($user->uid==1){
            $nameArr = user_load($row['uid']);
            $username = $nameArr->name;
        }
        
        $approve_link = "";
        $dateArr = explode(' ', $row['created_date']);
        $posted_date = dateformat($dateArr['0']);
        $viewlink = l('View', $links_admin.'tips_advices/view_tip/'.encrypt($row['id'],$key), array('attributes' => array('class' => 'popup', 'title'=>'View Tip')));
        $editlink = l('Edit', $links_admin.'tips_advices/edit_tip/'.encrypt($row['id'],$key));
        $dellink = l('Delete', $links_admin.'tips_advices', array('attributes' => array('onclick'=>"return del();"), 'query' => array ('del' => encrypt($row['id'],$key))));
        
        if(($user->uid==1 || in_array('faculty_admin', $user->roles)) && $row['tips_status']!=1)
            $approve_link = l('Approve', $links_admin.'tips_advices', array('attributes' => array('onclick'=>"return confirm('Are you sure you want to Approve this tip.')"), 'query' => array ('approve' => encrypt($row['id'],$key))));
        $links = '<div class="options">'.$approve_link.$viewlink.$editlink.$dellink.'</div>';
        
        if($user->uid==1)
            $result[] = array($row['tip_id'], $row['title'], $cat_tips_advice[$row['category']], ($article_type[$row['article_type']]), $posted_date, $username, $tips_status[$row['tips_status']], $links);
        else
            $result[] = array($row['tip_id'], $row['title'], $cat_tips_advice[$row['category']], ($article_type[$row['article_type']]), $posted_date, $tips_status[$row['tips_status']], $links);
    }
    
    /* Table header columns */  
    if($user->uid==1){
        $header = array(                 
            array('data' => t('Tip Id')),
            array('data' => t('Title')), 
            array('data' => t('Category')),
            array('data' => t('Type')),
            array('data' => t('Posted date')),
            array('data' => t('Name')),
            array('data' => t('status')),
            array('data' => t('Options')),
        ); 
    }else{  
        $header = array(
            array('data' => t('Tip Id')),
            array('data' => t('Title')), 
            array('data' => t('Category')),
            array('data' => t('Type')),
            array('data' => t('Posted date')),
            array('data' => t('status')),
            array('data' => t('Options')),
        );
    }
    
    // pagination script starts    
    $per_page = 10;
    // Initialize the pager
    $current_page = pager_default_initialize(count($result), $per_page);
    // Split your list into page sized chunks
    $chunks = array_chunk($result, $per_page, TRUE);
    // Show the appropriate items from the list
    @$output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page], 'empty' => t('No Records Found!')));
    // Show the pager
    $output .= theme('pager', array('quantity',count($result)));    
    
    $form['manage_header'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Manage Tip & Advices</h2>', 
        '#prefix' => '<div class="manage-tips">'
    );
        
    $form['keyword'] = array(
        '#title' => 'Title',        
        '#type' => 'textfield',        
        '#default_value' => (isset($form_state['input']['keyword']) ? $form_state['input']['keyword'] : ''),
    );
    
    $form['category'] = array(
        '#type' => 'select',
        '#title' => t('Category'),
        '#options' => array(''=>'Select') + $cat_tips_advice,        
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'tips_advice_general_advice_list_ajax',
            'wrapper' => 'general-advice-list-wrapper',
        ),
        '#default_value' => (isset($category) ? $category : ''),
    );
    
    // Wrapper for sub parent dropdown list
    $form['wrapper'] = array(
        '#prefix' => '<div id="general-advice-list-wrapper">',
        '#suffix' => '</div>',
    );
    
    if (isset($form_state['values']['category'])) {        
        if ($form_state['values']['category'] == "0") {
            $general_advice_list = general_advice_list();
            // Province dropdown list
            $form['wrapper']['general_advice'] = array(
                '#type' => 'select',
                '#title' => t('General Advice'),
                '#options' => $general_advice_list,
                '#default_value' => (isset($form_state['input']['general_advice']) ? $form_state['input']['general_advice'] : ''),
            );
        }else if ($form_state['values']['category'] == "1") {
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

            if (isset($form_state['values']['career_p']) && $form_state['values']['career_p'] != "") {
                $category_subparrents = db_select('specializations_sp')
                        ->fields('specializations_sp')
                        ->condition('spid', $form_state['values']['career_p'], '=')
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

            if (isset($form_state['values']['career_sp']) && $form_state['values']['career_sp'] != "") {
                $category_childs = db_select('specializations_c')
                        ->fields('specializations_c')
                        ->condition('cid', $form_state['values']['career_sp'], '=')
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
        }else if ($form_state['values']['category'] == "3") {
            $nature_businesss_arr = nature_bussiness_arr();
            // Province dropdown list
            $form['wrapper']['advice_industries'] = array(
                '#type' => 'select',
                '#title' => t('Advice by Industries'),
                '#options' => array('0'=>'Select') + $nature_businesss_arr,
                '#default_value' => (isset($form_state['input']['advice_industries']) ? $form_state['input']['advice_industries'] : ''),
            );
        }else{    
            $fields_of_study_arr = array(''=>'Select') + fields_of_study();
            $form['wrapper']['field_of_study'] = array(
                '#type' => 'select',
                '#title' => t('Field of Study'),
                '#options' => $fields_of_study_arr,
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

            if (isset($form_state['values']['field_of_study']) && $form_state['values']['field_of_study'] != "") {   
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

            if (isset($form_state['values']['qualification']) && $form_state['values']['qualification'] != "") {  
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
        }
    }
    
    $form['article_type'] = array(
        '#type' => 'select',
        '#title' => t('Article Type'), 
        '#options' => array(''=>'Select', t('Content'), t('Video')),               
        '#default_value' => (isset($form_state['input']['article_type']) ? $form_state['input']['article_type'] : ''),              
    );
    
    $form['daterange'] = array(
        '#type' => 'fieldset',
        '#title' => t('Date'),
    );
       
    $form['daterange']['from'] = array(
        '#type' => 'textfield', 
        '#id' => 'from', 
        '#title' => 'From',
        '#attributes' => array('readonly' => 'readonly'),
    );    
     
    $form['daterange']['to'] = array(
        '#type' => 'textfield', 
        '#id' => 'to', 
        '#title' => 'To',
        '#attributes' => array('readonly' => 'readonly'),
    );
    
    $form['status'] = array(
        '#type' => 'select',
        '#title' => t('Status'), 
        '#options' => array(''=>'Select') + tips_status(),               
        '#default_value' => (isset($form_state['input']['article_type']) ? $form_state['input']['article_type'] : ''),              
    );
    
    $form['buttons'] = array(
        '#type' => 'fieldset',
    );
    
    $form['buttons']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Search'),   
        '#submit' => array('manage_tips_advices_submit'),
    );
    
    $form['buttons']['reset'] = array(
        '#type' => 'button',
        '#button_type' => 'reset',
        '#value' => t('Clear'),
        '#weight' => 9,
        '#attributes' => array(
            'onclick' => 'this.form.reset(); return false;',
        ),
        '#suffix' => '</div>'
    );
    
    $form['cutomtext'] = array(
        '#type' => 'item',
        '#markup' => $output,
    );
    
    return $form; 
}

/**
 * Tips & Advice search submission
 */
function manage_tips_advices_submit($form, &$form_state) {      
    $form_state['rebuild'] = TRUE;
}