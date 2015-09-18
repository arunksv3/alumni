<?php
/**
 * @file
 * A tips and advice module that displays form to add tips and advices.
 */
function add_tips_advices($form, &$form_state, $editid){     
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery.validate.js');    
    drupal_add_js(drupal_get_path('module', 'tips_advices') .'/js/custom_validations.js');   
    
    $key = variable_get('IIUM_Encryption_Key', NULL);
    $cat_tips_advice = cat_tips_advice();
    $general_advice_list = general_advice_list();
    
    if($editid!=""){
        $query = db_select('tips_advices', 'tps');
        $query->fields('tps');    
        $query->condition('id', (int)decrypt($editid, $key), '=');        
        $result = $query->execute()->fetchAssoc(); 
        
        /* inline javascript */
        drupal_add_js(' 
            jQuery(document).ready(function($) { 
                '.((isset($editid) && $editid!="" ? '
                   showOtions('.$result["article_type"].')
                ' : '')).' 
                $("#edit-category").change();                                
                '.((isset($result['career_p']) && $result['career_p']!="0" ? '
                    setTimeout(function(){ 
                        $("#edit-career-p").change();
                    }, 500);
                ' : '')).'                 
                '.((isset($result['career_sp']) && $result['career_sp']!="0" ? '
                    setTimeout(function(){                     
                        $("#edit-career-sp").change();
                    }, 1200);
                ' : '')).'                                                        
                '.((isset($result['field_of_study']) && $result['field_of_study']!="0" ? '
                    setTimeout(function(){ 
                        $("#edit-field-of-study").change();
                    }, 500);
                ' : '')).' 
                '.((isset($result['qualification']) && $result['qualification']!="0" ? '
                    setTimeout(function(){ 
                        $("#edit-qualification").change();
                    }, 1200);
                ' : '')).' 
            });
        ', 'inline');
    }
    (!empty($result) ? extract($result) : '' );  
    
    //Image variables
    $image_var = array(
        'path' => $result['image_path'], 
        'alt' => $result['image_path'],
        'title' => $result['image_path'],  
        'width' => '150',
        'height' => '150', 
    );
    
    $form['tips_advices_heading'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Create Tip & Advices</h2>',        
    );
      
    $form['category'] = array(
        '#type' => 'select',
        '#title' => t('Category'),
        '#options' => array(''=>'Select') + $cat_tips_advice,
        '#required' => TRUE, 
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
            $general_advice_list =  array('0'=>'Select') + general_advice_list();
            // Province dropdown list
            $form['wrapper']['general_advice'] = array(
                '#type' => 'select',
                '#title' => t('General Advice'),
                '#options' => $general_advice_list,
                '#default_value' => (isset($general_advice) ? $general_advice : ''),
            );
        }else if ($form_state['values']['category'] == "1") {
            $category_parrents = db_select('specializations_p')
                    ->fields('specializations_p')
                    ->execute()
                    ->fetchAll();
            $display_category_parrents['0'] = 'All Specializations';
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
                '#default_value' => (isset($career_p) ? $career_p : ''),
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
                $display_category_subparrents['0'] = 'All';
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
                    '#default_value' => (isset($career_sp) ? $career_sp : ''),
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
                $display_category_childs['0'] = 'All';
                foreach ($category_childs as $category_child_name):
                    $display_category_childs[$category_child_name->sid] = $category_child_name->name;
                endforeach;
                // Province dropdown list
                $form['wrapper']['wrapper_parent']['wrapper_child']['career_c'] = array(
                    '#type' => 'select',
                    '#options' => $display_category_childs,
                    '#default_value' => (isset($career_c) ? $career_c : ''),
                );
            }
        }else if ($form_state['values']['category'] == "3") {
            $nature_businesss_arr = nature_bussiness_arr();
            // Province dropdown list
            $form['wrapper']['advice_industries'] = array(
                '#type' => 'select',
                '#title' => t('Advice by Industries'),
                '#options' => array('0'=>'Select') + $nature_businesss_arr,
                '#default_value' => (isset($advice_industries) ? $advice_industries : ''),
            );
        }else{    
            $fields_of_study_arr = array('0'=>'Select') + fields_of_study();
            $form['wrapper']['field_of_study'] = array(
                '#type' => 'select',
                '#title' => t('Field of Study'),
                '#options' => $fields_of_study_arr,
                '#ajax' => array(
                    'event' => 'change',
                    'callback' => 'tips_advice_fields_of_study_list_ajax',
                    'wrapper' => 'tips-advice-field-of-study-list-wrapper',
                ),
                '#default_value' => (isset($field_of_study) ? $field_of_study : ''),
            );

            // Wrapper for sub parent dropdown list
            $form['wrapper']['wrapper_field_of_study'] = array(
                '#prefix' => '<div id="tips-advice-field-of-study-list-wrapper">',
                '#suffix' => '</div>',
            );

            if (isset($form_state['values']['field_of_study']) && $form_state['values']['field_of_study'] != "") {   
                $qualification_list = array('0'=>'Select') + qualification_list();
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
                    '#default_value' => (isset($qualification) ? $qualification : ''),
                );
            }

            // Wrapper for child dropdown list
            $form['wrapper']['wrapper_field_of_study']['wrapper_qualification'] = array(
                '#prefix' => '<div id="qualification-list-wrapper">',
                '#suffix' => '</div>',
            );

            if (isset($form_state['values']['qualification']) && $form_state['values']['qualification'] != "") {  
                $qid = $form_state['input']['qualification'];                
                $qualification_level_arr = array('0'=>'Select') + highest_qualification_list($qid);
                // Province dropdown list
                $form['wrapper']['wrapper_field_of_study']['wrapper_qualification']['qualification_level'] = array(
                    '#type' => 'select',
                    '#title' => t('Qualification Level'),
                    '#options' => $qualification_level_arr,
                    '#default_value' => (isset($qualification_level) ? $qualification_level : ''),
                );
            }
        }
    }
    
    $form['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'), 
        '#required' => TRUE,
        '#default_value' => (isset($title) ? $title : ''),       
    ); 
    
    $form['article_type'] = array(
        '#type' => 'radios',
        '#title' => t('Article Type'), 
        '#required' => TRUE, 
        '#options' => array(t('Content'), t('Video')),               
        '#default_value' => (isset($article_type) ? $article_type : '0'),       
        '#attributes' => array('onclick' => "showOtions(this.value)"), 
    );
    
    $form['image_name'] = array(
        '#title' => 'Upload Image to this article <span title="This field is required." class="form-required">*</span>',        
        '#type' => 'file',
        '#description' => 'Allowed extensions (.png, .jpeg, .gif)',        
        '#prefix' => '<div class="" id="article_image">',
        '#suffix' => (isset($image_path) ? theme('image', $image_var) : '').'</div>',
    );        
    
    $form['embded_code'] = array(
        '#title' => 'Paste the video embded code here <span title="This field is required." class="form-required">*</span>',        
        '#type' => 'textarea',    
        '#prefix' => '<div style="display:none" id="article_video">',
        '#suffix' => '</div>',
        '#default_value' => (isset($embded_code) ? $embded_code : ''), 
    );    
    
    $form['content_desc'] = array(
        '#title' => 'Description',        
        '#type' => 'text_format',       
        '#format' => filter_default_format(),
        '#required' => TRUE, 
        '#default_value' => (isset($content_desc) ? $content_desc : ''), 
    );  
    
    if($result['attachment']!=""){
        $attachArr = explode('/',$result['attachment']);
        $attchment_name = end($attachArr);
    }
    
    $form['attachment'] = array(
        '#title' => 'Attachment',        
        '#type' => 'file',
        '#description' => 'Allowed extensions (.html, .ppt, .pptx, .doc, .docx)',        
        '#prefix' => '<div class="form-item form-type-file form-item-files-company-logo">',
        '#suffix' => (isset($attchment_name) ? l(t('Download Attachment'), 'tips_advices/download/'.encrypt($attchment_name, $key))
                : '').'</div>',           
    );
    
    $form['#attributes'] = array('enctype' => 'multipart/form-data', 'id'=>'add-tips-advices');
    
    if ($editid != "") {
        $form['editid'] = array(
            '#type' => 'hidden',
            '#value' => $editid,
        );
    }
    
    $form['buttons'] = array(
        '#type' => 'fieldset',
    );
    
    $form['buttons']['cancel'] = array(
        '#prefix' => '<div class="cancelbtn">',
        '#markup' => l(t('Cancel'), $base_url.get_admin_link().'tips_advices'),
        '#suffix' => '</div>'
    ); 
    
    $form['buttons']['submit'] = array(
        '#type' => 'submit',
        '#title' => 'Submit',
        '#value' => t('Submit'),
        '#submit' => array('add_tips_advices_submit')
    );
    
    return $form; 
}

/**
 * Validate handler for add_tips_advice_validate().
 */
function add_tips_advices_validate($form, &$form_state) {   
    extract($form_state['input']);
    
    // file extension validation
    $image_name = $_FILES["files"]["name"]['image_name'];
    $attachment_name = $_FILES["files"]["name"]['attachment'];
    $image_allowed_exts =  array('gif','png' ,'jpg', 'jpeg');
    $attachment_allowed_exts =  array('html','ppt' ,'pptx', 'doc', 'docx');
    
    if($article_type=="0"){
        if($image_name!=""){
            $image_ext = pathinfo($image_name, PATHINFO_EXTENSION);        
            if(!in_array($image_ext, $image_allowed_exts) ) {
                form_set_error('image_name', t('Invalid file extension for article image'));
            }
        }else if($editid=="")
            form_set_error('image_name', t('Please upload image for article'));
    }
    
    if($attachment_name!=""){        
        $attachment_ext = pathinfo($attachment_name, PATHINFO_EXTENSION);        
        if(!in_array($attachment_ext, $attachment_allowed_exts) ) {
            form_set_error('attachment', t('Invalid file extension for Attachment'));
        }
    }
}

function add_tips_advices_submit($form, &$form_state){
    global $user;
    global $base_url;
    $key = variable_get('IIUM_Encryption_Key', NULL);    
    extract($form_state['input']);    
    $links_admin = "";
    $dest_image_path = "";
    $dest_attachment_path = "";  
    if($editid!="")
        $editid = (int)decrypt($editid, $key);
    
    /* file names */
    $image_name = $_FILES["files"]["name"]['image_name'];
    $attachment_name = $_FILES["files"]["name"]['attachment'];
    
    if($user->uid==1 || in_array('faculty_admin', $user->roles))
        $links_admin = 'admin/config/people/'; 
    
    if($editid==""){
        $query = db_select('ldap_user_info', 'ldap_info');
        $query->fields('ldap_info',array('uid','givenname'));    
        $query->condition('uid', $user->uid, '=');        
        $result = $query->execute()->fetchAssoc();
        $author_name = $result['givenname'];
        $db_status = 'db_insert';
    }else{ 
        $query = db_select('tips_advices', 'tps');
        $query->fields('tps',array('id','uid','author_name','image_name','image_path','attachment'));    
        $query->condition('id', $editid, '=');        
        $result = $query->execute()->fetchAssoc(); 
        $author_name = $result['author_name'];
        $db_status = 'db_update';
        
        $dest_image_path = $result['image_path'];
        if($image_name != $result['image_name'] && $image_name!="")
            unlink($result['image_path']);
        
        $dest_attachment_path = $result['attachment'];
        if($attachment_name != $result['attachment'] && $attachment_name!="")
            unlink($result['attachment']);
    }
    
    
    // Move files to destination folder
    if($image_name!=""){ 
        /* temp location names */
        $image_name_temp = $_FILES["files"]["tmp_name"]['image_name'];
        $dest_image_path = variable_get('IIUM_tips_advice_image_path', NULL).time().'_'.$image_name;        
        move_uploaded_file($image_name_temp, $dest_image_path);
    }    
    
    if($attachment_name!=""){
        $attachment_name_temp = $_FILES["files"]["tmp_name"]['attachment']; 
        $dest_attachment_path = variable_get('IIUM_tips_advice_attachment_path', NULL).time().'_'.$attachment_name;
        move_uploaded_file($attachment_name_temp, $dest_attachment_path);
    }
    
    if($article_type=='0'){         
        $embded_code = "";
    }
    
    // inserting values to tips_advices table
    $tid = $db_status('tips_advices');
    $tid->fields(array(
            'uid' => $user->uid,
            'author_name' => $author_name,
	    'title' => $form_state['input']['title'],
            'category' => $category,
            'general_advice' => $general_advice,
            'career_p' => $career_p,
            'career_sp' => $career_sp,
	    'career_c' => $career_c,
            'field_of_study' => $field_of_study,
            'qualification' => $qualification,
            'qualification_level' => $qualification_level,
            'advice_industries' => $advice_industries,
            'article_type' => $article_type,
	    'content_desc' => $form_state['input']['content_desc']['value'],
            'embded_code' => $embded_code,
            'image_name' => $image_name,
            'image_path' => $dest_image_path,
            'attachment' => $dest_attachment_path,
        ));
        if($editid!=""){
            $tid->condition('id', $editid, "=");
            $tips_advices_id=tips_advices_id($editid);
            audit_log('Updated Tips and Advice', "Tip ID: ".$tips_advices_id->tip_id);
        }
    $tid = $tid->execute();
    
    

    if($editid==""){
        /* Insert tip id, start */
        if ($tid <= 9)
            $value = "000" . $tid;
        else if ($tid <= 99 && $tid > 9)
            $value = "00" . $tid;
        else if ($tid <= 999 && $tid > 99)
            $value = "0" . $tid;
        else
            $value = $tid;

        $tipid = 'TP-' . $value;

        $fields = array(
            'tip_id' => $tipid,
        );

        $employer_jobs_update = db_update('tips_advices')
            ->fields($fields)
            ->condition('id', $tid)
            ->execute();
    
        audit_log('Created Tips and Advice', "Tip ID: ".$tipid);
        
        $path = $base_url.'/'.drupal_get_path('theme', 'bootstrap_business'); 
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
                    <td>Hi Admin,</td>
                </tr>
                <tr>
                    <td>The Tip '.$tipid.' is waiting for your approval. Please login to your interface and review the posted Tips</td>
                </tr>
                <tr>
                    <td>Regards, </br>
                    IIUM Career Consuling Team</td>
                </tr>
            </table>    
        </body>
        </html>';

        $my_module = 'tips_advices';
        $my_mail_token = 'tips_advice_waiting_approval';
        $from = variable_get('site_mail');
        $subject = "Waiting approval for Tips & Advice";

        $message = array(
            'to' => $from,
            'subject' => $subject,
            'body' => $body,
            'headers' => array(
                'From' => $user->mail,
                'MIME-Version' => '1.0',
                'Content-Type' => 'text/html;charset=utf-8'),
        );

        //Drupal mail function core
        $system = drupal_mail_system($my_module, $my_mail_token);        
        if ($system->mail($message)) {   
            drupal_set_message(t('Thank you for posting your valubale Tips & Advices for our candidates. This posting will be displayed over the website after admin approval. You will notified the same.'));              
        }
    }else{
        drupal_set_message(t('Tip has been updated successfully'));
    }
    
    drupal_goto($links_admin.'tips_advices');
}