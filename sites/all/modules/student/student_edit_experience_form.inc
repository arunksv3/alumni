<?php 
function student_edit_experience_form($form, &$form_state, $eid){
    global $base_url;
    global $user;
    $location_country = 0;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery.validate.js');    
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'student') .'/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
    drupal_add_js('jQuery(document).ready(function($) { 
        $("#date_left").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2015",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        $("#date_joined").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2015",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        

        $("#date_left1").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2015",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        $("#date_joined1").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2015",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
       
        $(".popup").click(function () {                    
          window.open($(this).attr("href"), "PopupWindow", "width=600,height=600,scrollbars=yes,resizable=no");
          return false;
        });
        });', 'inline');
     
   
    $student_details = db_select('student_details')
        ->fields('student_details')    
        ->condition('uid', $user->uid, '=')
        ->execute();  
    while($row = $student_details->fetchAssoc($student_details)){
        $level = $row['experience_level'];
        $year = $row['working_year'];
        $month = $row['working_month'];
        $monthArr = getMonth(); 
        if($level==0 && ($year>0 || $month>0))
            $experience_level = 'I have been working since '.$monthArr[$month].' '.$year;
        else if($row['experience_level']==1)
            $experience_level = 'Fresh graduate seeking my first job';
        else if($row['experience_level']==2)
            $experience_level = 'Student seeking internship or part-time jobs ';
        else
            $experience_level = '';
    }
    $form = array();
           
if($eid=="")
{
    $form['title'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>Experience Level</h2> '
    );
    
    $form['experience_level'] = array(
        '#type' => 'fieldset', 
        '#prefix' => '<div id="addExperienceLevel">',
        '#suffix' => '</div>',
    ); 
      $form['experience_level']['level'] = array(
        '#type' => 'item', 
        '#markup' => ' 
        <div id="Education1" class="row" >                  
             
            <div class="col-md-10 resume_mid">
            <ul><li><label>Experience Level: </label><span> '.$experience_level.'</span></li></ul>
            </div>
            <div id="ExperienceLevelEdit" class="col-md-2 resume_right">
            <i id="icon-edit" class="icon-edit"></i>
            </div>
        </div>
           ',
    ); 
   
    $form['experience_level1'] = array(
        '#type' => 'fieldset', 
        '#prefix' => '<div id="editExperienceLevel" style="display:none">',
        '#suffix' => '</div>',
    );
    $form['experience_level1']['level'] = array(
            '#type' => 'radios',
            '#id'=>'experience_level',
            '#title' => t(''), 
            '#options' => array(1 => t('I am a fresh graduate seeking my first job'),
                                2 => t('I am a student seeking internship or part-time jobs'),
                                0 => t('I have been working since')
                            ),
           // '#required' => TRUE,
            '#default_value' => $level,
           
            '#prefix' => '<div>',
            '#suffix' => '</div>',
        ); 
    if($level==0) {
        $style = 'style="display:block"';
    } else {
        $style = 'style="display:none"';
    }
    $form['experience_level1']['year'] = array(
        '#id' => 'year',
        '#type' => 'select',
        '#options' =>  array(''=>'Select') + getYear(),
        '#default_value' => $year,  
        '#prefix' => '<div id="WorkingDate" '.$style.'>',
    );
    
    $form['experience_level1']['month'] = array(
        '#id' => 'month',
        '#type' => 'select',
        '#options' =>  array(''=>'Select') + getMonth(),
        '#default_value' => $month,
        '#suffix' => '</div>',
    );
    
    $form['experience_level1']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Submit'),  
            '#submit' => array('student_edit_experience_form_submit_add'),
            '#prefix' => '<div class="submit_clear_but">',
           // '#suffix' => '</div>',
        );
        
    $form['experience_level1']['cancel_skills'] = array(
        '#id' => 'Cancel',
        '#type' => 'button',
        '#value' => t('Cancel'),         
        '#suffix' => '</div>',
        );
    
    /*$form['experience_level1']['row'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>&nbsp;</h2> '
        );*/
    
    $form['seperator'] = array(
        '#type' => 'item', 
        '#markup' => '<div class="seperator">',
        '#suffix' => '</div>',
    ); 


    $form['addExperience'] = array(
        '#type' => 'fieldset', 
        '#title' => t('Add'), 
        '#prefix' => '<div id="addExperienceDiv">',
        '#suffix' => '</div>',
    ); 

    // To make these fields required
        $student_experience = db_select('student_experience')
        ->fields('student_experience')    
        ->condition('uid', $user->uid, '=')
        ->orderBy('id', 'DESC')
        ->execute();    
    $count = $student_experience->rowCount(); 
    if($count>0) { 
       $form['experience2'] = array(
        '#type' => 'fieldset', 
        //'#title' => t('Work Experience'), 
        '#prefix' => '<div id="viewExperienceDiv">',
       // '#suffix' => '</div>',
    ); 
       $output='';
      
    while($row = $student_experience->fetchAssoc($student_experience)){
        $company_name = $row['company_name'];
        $date1 = explode('-', $row['date_joined']);
        $present = $row['present'];
        $date_joined = $date1[1].'/'.$date1[2].'/'.$date1[0];   
        if($row['date_left']!=''){
        $date2 = explode('-', $row['date_left']);
        $date_left = $date2[1].'/'.$date2[2].'/'.$date2[0]; 
        } else{
            $date_left = '';
        }
        if($present==1)
            $duration = date_diff(date_create($date_joined), date_create(date('Y-m-d')))->y; 
        else
            $duration = date_diff(date_create($date_joined), date_create($date_left))->y; 
        $job_title = $row['job_title'];
        $position = $row['position_level'];
        $location_country = $row['country'];
        $location_state = $row['state'];
        $salary = $row['salary']; 
        $description = $row['description']; 
        $specializationp= $row['specialization_p'];
        $specializationsp= $row['specialization_sp'];
        $specializationc = $row['specialization_c'];
        //$additional_info = $row['additional_info']; 
        $industry = $row['industry']; 
        $type = $row['experience_level']; 
        $id = $row['id'];
        $industryArr = nature_bussiness_arr();
        $output.='<div id="Education1" class="row" >  
                <div class="col-md-2 resume_left">';
        if($present==1)
            $output.='<div>'.date('M Y',strtotime($date_joined)).' - Present </div>';
        else
            $output.='<div>'.date('M Y',strtotime($date_joined)).' - '.date('M Y',strtotime($date_left)).' </div>';

        $output.='<div>('.$duration.' Years)</div>
                </div>
                <div class="col-md-8 resume_mid">
                <h1>'.$job_title.'</h1>
                    <h2>'.$company_name.' | '.get_country_name($location_country).' '.get_state_name($location_state).'</h2>
                <ul class="working_list">
                    
                    <li><label>Industry  </label><span>'.$industryArr[$industry].'</span></li>                 
                    <li><label>Specialization </label><span>'.getSpecializationParentName($specializationp).' ('.getSpecializationSubparent($specializationsp).')</span></li>                    
                    <li><label>Role </label><span>'.getSpecializationChild($specializationc).'</span></li>
                    
                    <li><label>Position Level  </label><span>'.get_position_name($position).'</span></li>
                    <li><label>Monthly Salary (MYR) </label><span>'.$salary.'</span></li>
                    <li><label>&nbsp;</label><span>'.$description.'</span></li>
                </ul>                
                </div>
                <div id="edit_experience" class="col-md-2 resume_right">
                <a href="'.$base_url.'/student/editexperience/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/edit"><i id="icon-edit" class="icon-edit"></i></a>
                &nbsp;&nbsp;<a  onclick="return confirm(\'Delete Experience?\')" href="'.$base_url.'/student/deleteexperience/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/delete"><i id="icon-delete" class="icon-delete"></i></a>
                 </div>
                </div>'; 
        
    }
    
    $form['edit_skills']['skills'] = array(
            '#type' => 'item',
            '#markup' => $output,
        );
}
}
     
    $form['experience'] = array(
        '#type' => 'fieldset', 
        //'#title' => t('Work Experience'), 
        '#prefix' => '</div><div id="Edit_Experience_Div" style="display:none">',
        '#suffix' => '</div>',
    );  
    
    $form['experience']['title'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>Work Experience</h2>', 
       
    );  
 
    $form['experience']['company_name'] = array(
        '#type' => 'textfield',
        '#id' => 'company_name',
        '#title' => t('Company Name <span class="form-required">*</span>'),
       // '#required' => TRUE, // Added
        '#default_value' => '',  
    );
   
    
    $form['experience']['industry'] = array(
        '#type' => 'select',
        '#id' => 'industry',
        '#title' => t('Industry <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + nature_bussiness_arr(),
                
        //'#required' => TRUE,
        '#value' => '', 
    ); 

    $form['experience']['date_joined'] = array(
     '#type' => 'textfield', 
     '#id' => 'date_joined',
     '#title' => 'Date Joined <span class="form-required">*</span>',
     //'#required' => TRUE, // Added
    '#attributes' => array('readonly' => 'readonly'),
     '#default_value' => '',  
     );
      
    
     $form['experience']['date_left'] = array(
        '#title' => 'Date Left <span class="form-required">*</span>',  
        '#id' => 'date_left',
        '#type' => 'textfield',
       // '#required' => TRUE,
         '#attributes' => array('readonly' => 'readonly'),
        '#default_value' =>'',  
        '#maxlength' => 14
    );
     
     
    $form['experience']['or'] = array(           
            '#title' => 'or',
            '#type' => 'item',
    );
      
      $form['experience']['present'] = array(
            '#id' => 'present',
            '#title' => 'Present (Currently Working)',
            '#type' => 'checkbox',
           // '#value' => 1,
           // '#default_value' => (isset($present) ? $present : ''),  
        ); 
     
      $form['experience']['job_title'] = array(
        '#title' => 'Job Title <span class="form-required">*</span>',    
        '#id' => 'company_name',
        '#type' => 'textfield',
       // '#required' => TRUE,
        '#default_value' => '',  
       // '#maxlength' => 14
    );
     
    /* $form['experience']['position'] = array(
     '#type' => 'textfield', 
     '#title' => 'Position Level',
     '#required' => TRUE, // Added
     '#default_value' =>'',
     '#prefix' => '<div style="clear:both">',
     '#suffix' => '</div>',
     );*/
     
     
     $form['experience']['position'] = array(
     '#type' => 'select', 
     '#id' => 'position',
     '#options' => array(''=>'Select') + positionoffered(),
     '#title' => 'Position Level <span class="form-required">*</span>',
     //'#required' => TRUE, // Added
     '#default_value' => (isset($position) ? $position : ''),     
     );
    
     
     $category_parrents = db_select('specializations_p')
                           ->fields('specializations_p') 
                           ->execute()
                           ->fetchAll(); 
    $display_category_parrents['']= 'All Specializations';   
    foreach($category_parrents as $category_parrent_name): 
      $display_category_parrents[$category_parrent_name->sid]= $category_parrent_name->name;   
    endforeach;
    //echo $specializationp;
            
    $form['experience']['filter']['specializationp'] = array(
      '#type' => 'select',
    '#id' => 'specializationp',
     //'#default'=>$specializationp,   
      '#title' => t('Specialization <span class="form-required">*</span>'),
      '#options' => $display_category_parrents,
      '#ajax' => array(
       'event'=>'change',
       'callback' => 'student_subparrent_list_ajax',
       'wrapper' => 'subparrent-list-wrapper',
     ),
    );
    // Wrapper for sub parent dropdown list
    $form['experience']['filter']['wrapper'] = array(
     '#prefix' => '<div id="subparrent-list-wrapper">',
     '#suffix' => '</div>',
    );
    if (isset($form_state['values']['specializationp'])&& $form_state['values']['specializationp']!="") {
        $category_subparrents = db_select('specializations_sp')
                           ->fields('specializations_sp') 
                           ->condition('spid',$form_state['values']['specializationp'], '=')   
                           ->execute()
                           ->fetchAll(); 
        $display_category_subparrents['']= 'Select';   
        foreach($category_subparrents as $category_subparrent_name): 
          $display_category_subparrents[$category_subparrent_name->sid]= $category_subparrent_name->name;   
        endforeach;
         // Province dropdown list
        $form['experience']['filter']['wrapper']['specializationsp'] = array(
            '#id' => 'specializationsp',
            '#type' => 'select',
            '#options' => $display_category_subparrents,
            '#ajax' => array(
                'event'=>'change',
                'callback' => 'student_child_list_ajax',
                'wrapper' => 'child-list-wrapper',
            ),
        );         
    }
    // Wrapper for child dropdown list
        $form['experience']['filter']['wrapper_child'] = array(
            '#prefix' => '<div id="child-list-wrapper">',
            '#suffix' => '</div>',
         ); 
     
        if (isset($form_state['values']['specializationsp']) && $form_state['values']['specializationsp'] != "") {
            $sub_parent = $form_state['values']['specializationsp'];
            $category_childs = db_select('specializations_c')
                ->fields('specializations_c') 
                ->condition('cid',$sub_parent, '=')   
                ->execute()
                ->fetchAll(); 
            $display_category_childs['']= 'Select';   
            foreach($category_childs as $category_child_name): 
              $display_category_childs[$category_child_name->sid]= $category_child_name->name;   
            endforeach;
             // Province dropdown list
         $form['experience']['filter']['wrapper_child']['specializationc'] = array(
             '#id' => 'specializationc',
            '#title' => 'Role <span class="form-required">*</span>',
             '#type' => 'select',
            '#options' => $display_category_childs,
         );         
        }
        
        $form['experience']['location_country'] = array(
         '#id' => 'location_country',
        '#type' => 'select',
        '#title' => t('Work Location <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_location_state_list_ajax1',
            'wrapper' => 'loction-state-list-wrapper1',
        ),
        '#default_value' => (isset($location_country) ? $location_country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['experience']['location']['wrapper'] = array(
        '#prefix' => '<div id="loction-state-list-wrapper1">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['location_country']) && $form_state['values']['location_country'] != "") || $location_country>0) {
        $cid = isset($form_state['values']['location_country'])?$form_state['values']['location_country']:$location_country;
        $state_list = db_select('states_list')
                ->fields('states_list')
                ->condition('cid', $cid, '=')
                ->execute()
                ->fetchAll();
        $state_list_arr[''] = 'Select';
        foreach ($state_list as $value):
            $state_list_arr[$value->id] = $value->name;
        endforeach;
        if(count($state_list) > 0){
            // Province dropdown list
            $form['experience']['location']['wrapper']['location_state'] = array(
                '#id' => 'location_state',
                '#type' => 'select',
                '#title' => t('State <span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($location_state) ? $location_state : ''),                
            );
        }
    }
 
        $form['experience']['salary'] = array(
           '#id' => 'salary',
           '#type' => 'textfield',
           '#title' => 'Monthly Salary (MYR)<span class="form-required">*</span>',
           //'#required' => TRUE, // Added
           '#default_value' => (isset($experience_details->salary) ? $experience_details->salary: ''),  
        );
        $form['experience']['description'] = array(
           '#type' => 'text_format', 
           '#title' => 'Work Description',
           //'#required' => TRUE, // Added
           '#default_value' => '',  
        );     
         $form['buttonsField'] = array(
        '#type' => 'fieldset', 
        '#title' => t(''),
        '#prefix' => '<div id="buttonsDiv" style="display:none" >',
        '#suffix' => '</div>',
        );
         
        $form['buttonsField']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Add'),  
            '#submit' => array('student_edit_experience_form_submit_add'),
            '#prefix' => '<div class="submit_clear_but">',
           // '#suffix' => '</div>',
        );
         
         
         $form['buttonsField']['cancel'] = array(
            '#type' => 'item', 
            '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/student/editexperience">Cancel</a>'),
            '#suffix' => '</div>',
        );
        
         /*$form['buttonsField']['cancel'] = array(
            '#type' => 'fieldset',
            '#value' => t('Cancel'),   
            '#prefix' => '<div id="hideExperienceDiv" style="cursor:pointer;background-color:#663398;width:100px;padding:10px;color:#FFF">',
            '#suffix' => '</div>',
        );
 */
         
        
        if($eid!="") {
            $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
            $eid=(int)decrypt($eid,$decrypting_key);
            $experience_details=  editexperience($eid);
         
            $date1 = explode('-', $experience_details->date_joined);
        $date_joined = $date1[1].'/'.$date1[2].'/'.$date1[0];     
        if($experience_details->date_left!=''){
            $date2 = explode('-', $experience_details->date_left);
            $date_left = $date2[1].'/'.$date2[2].'/'.$date2[0]; 
        } else{
            $date_left = '';
        }
        $present = $experience_details->present;
        $specializationp = $experience_details->specialization_p;
        $specializationsp = $experience_details->specialization_sp;
        $specializationc = $experience_details->specialization_c;
        $location_country = $experience_details->country;
        $location_state = $experience_details->state;
    $form['experience1'] = array(
        '#type' => 'fieldset', 
      //  '#title' => t('Work Experience'), 
        '#prefix' => '<div id="ExperienceDiv1">',
        '#suffix' => '</div>',
    ); 
    
    $form['experience1']['title'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>Work Experience</h2>',        
    ); 
 
    $form['experience1']['company_name'] = array(
        '#id' => 'company_name1',
        '#type' => 'textfield',
        '#title' => t('Company Name <span class="form-required">*</span>'),
      //  '#required' => TRUE, // Added
        '#default_value' => (isset($experience_details->company_name) ? $experience_details->company_name : ''),  
    );
   
    
    $form['experience1']['industry'] = array(
        '#id' => 'industry1',
        '#type' => 'select',
        '#title' => t('Industry <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + nature_bussiness_arr(),
        '#attributes' => array(
            'name' => 'industry[]'
        ),        
       // '#required' => TRUE,
        '#value' => (isset($experience_details->industry) ? $experience_details->industry : ''), 
    ); 

    $form['experience1']['date_joined'] = array(
     '#type' => 'textfield', 
     '#id' => 'date_joined1',
     '#title' => 'Date Joined <span class="form-required">*</span>',
        '#attributes' => array('readonly' => 'readonly'),
    // '#required' => TRUE, // Added
     '#default_value' => (isset($date_joined) ? $date_joined : ''),  
     );
      
    if($present==1){
        $form['experience1']['date_left'] = array(
           '#title' => 'Date Left <span class="form-required">*</span>',  
           '#id' => 'date_left1',
           '#type' => 'textfield',
            '#attributes' => array('readonly' => 'readonly'),
           '#attributes' => array('disabled'=>'disabled'),
           '#default_value' => (isset($date_left) ? $date_left : ''),  
           '#maxlength' => 14
       );
    } else {
        $form['experience1']['date_left'] = array(
           '#title' => 'Date Left <span class="form-required">*</span>',  
           '#id' => 'date_left1',
           '#type' => 'textfield',
            '#attributes' => array('readonly' => 'readonly'),
           '#default_value' => (isset($date_left) ? $date_left : ''),  
           '#maxlength' => 14
       );
    }
    $form['experience1']['or'] = array(           
            '#title' => 'or',
            '#type' => 'item',
    );
     $form['experience1']['present'] = array(
            '#id' => 'present1',
            '#title' => 'Present (Currently Working)',
            '#type' => 'checkbox',
           // '#value' => 1,
            '#default_value' => (isset($present) ? $present : ''),  
    );
     
      $form['experience1']['job_title'] = array(
        '#title' => 'Job Title <span class="form-required">*</span>',        
        '#type' => 'textfield',
          '#id' => 'job_title1',
      //  '#required' => TRUE,
        '#default_value' => (isset($experience_details->job_title) ? $experience_details->job_title : ''),  
      //  '#maxlength' => 14
    );
    
       $form['experience1']['position'] = array(
     '#id' => 'position1', 
     '#type' => 'select', 
     '#options' => array(''=>'Select') + positionoffered(),
     '#title' => 'Position Level <span class="form-required">*</span>',
     //'#required' => TRUE, // Added
     '#default_value' => (isset($experience_details->position_level) ? $experience_details->position_level : ''),     
     );
     
     
     $category_parrents = db_select('specializations_p')
                           ->fields('specializations_p') 
                           ->execute()
                           ->fetchAll(); 
    $display_category_parrents['']= 'All Specializations';   
    foreach($category_parrents as $category_parrent_name): 
      $display_category_parrents[$category_parrent_name->sid]= $category_parrent_name->name;   
    endforeach;
    
    $form['experience1']['filter']['specializationp'] = array(
      '#id' => 'specializationp1',
      '#type' => 'select',
      '#title' => t('Specialization <span class="form-required">*</span>'),      
      '#options' => $display_category_parrents,
      '#ajax' => array(
       'event'=>'change',
       'callback' => 'studentexp_subparrent_list_ajax',
       'wrapper' => 'subparrent-list-wrapper1',
     ),
        '#default_value' => $specializationp, 
    );
    // Wrapper for sub parent dropdown list
    $form['experience1']['filter']['wrapper'] = array(
     '#prefix' => '<div id="subparrent-list-wrapper1">',
     '#suffix' => '</div>',
    );
    if (isset($form_state['values']['specializationp'])&& $form_state['values']['specializationp']!="" || $specializationp>0) {
        $spid = isset($form_state['values']['specializationp'])?$form_state['values']['specializationp']:$specializationp;
        $category_subparrents = db_select('specializations_sp')
                           ->fields('specializations_sp') 
                           ->condition('spid',$spid, '=')   
                           ->execute()
                           ->fetchAll(); 
        $display_category_subparrents['']= 'Select';   
        foreach($category_subparrents as $category_subparrent_name): 
          $display_category_subparrents[$category_subparrent_name->sid]= $category_subparrent_name->name;   
        endforeach;
         // Province dropdown list
        $form['experience1']['filter']['wrapper']['specializationsp'] = array(
            '#id' => 'specializationsp1',
            '#type' => 'select',
            '#options' => $display_category_subparrents,
            '#ajax' => array(
                'event'=>'change',
                'callback' => 'studentexp_child_list_ajax',
                'wrapper' => 'child-list-wrapper1',
            ),
            '#default_value' => $specializationsp, 
        );         
    }
    // Wrapper for child dropdown list
        $form['experience1']['filter']['wrapper_child'] = array(
            '#prefix' => '<div id="child-list-wrapper1">',
            '#suffix' => '</div>',
         );
        $sub_parent = array();
        if(isset($form_state['values']['specializationsp']) || $specializationsp>0){
            $sub_parent = isset($form_state['values']['specializationsp'])?$form_state['values']['specializationsp']:$specializationsp;
        }
       
        if (isset($sub_parent)&& $sub_parent!="") {
            $category_childs = db_select('specializations_c')
                               ->fields('specializations_c') 
                               ->condition('cid',$sub_parent, '=')   
                               ->execute()
                               ->fetchAll(); 
            $display_category_childs['']= 'Select';   
            foreach($category_childs as $category_child_name): 
              $display_category_childs[$category_child_name->sid]= $category_child_name->name;   
            endforeach;
             // Province dropdown list
         $form['experience1']['filter']['wrapper_child']['specializationc'] = array(
             '#id' => 'specializationc1',
             '#title' => 'Role <span class="form-required">*</span>',
            '#type' => 'select',
            '#options' => $display_category_childs,
             '#default_value' => $specializationc, 
         );    
         
        }
     
        $form['experience1']['location_country'] = array(
         '#id' => 'location_country1',
        '#type' => 'select',
        '#title' => t('Work Location <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_location_state_list_ajax2',
            'wrapper' => 'location-state-list-wrapper2',
        ),
        '#default_value' => (isset($location_country) ? $location_country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['experience1']['location']['wrapper'] = array(
        '#prefix' => '<div id="location-state-list-wrapper2">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['location_country']) && $form_state['values']['location_country'] != "") || $location_country>0) {        
        $cid = isset($form_state['values']['location_country'])?$form_state['values']['location_country']:$location_country;
        $state_list = db_select('states_list')
                ->fields('states_list')
                ->condition('cid', $cid, '=')
                ->execute()
                ->fetchAll();
        $state_list_arr[''] = 'Select';
        foreach ($state_list as $value):
            $state_list_arr[$value->id] = $value->name;
        endforeach;
        if(count($state_list) > 0){
            // Province dropdown list
            $form['experience1']['location']['wrapper']['location_state'] = array(
                '#id' => 'location_state1',
                '#type' => 'select',
                '#title' => t('State <span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($location_state) ? $location_state : ''),                
            );
        }
    }
     
        $form['experience1']['salary'] = array(
            '#id' => 'salary1',
           '#type' => 'textfield', 
           '#title' => 'Monthly Salary (MYR)<span class="form-required">*</span>',
           //'#required' => TRUE, // Added
           '#default_value' => (isset($experience_details->salary) ? $experience_details->salary: ''),  
        );
    
        $form['experience1']['description'] = array(
           '#type' => 'text_format', 
           '#title' => 'Work Description',
           //'#required' => TRUE, // Added
           '#default_value' => (isset($experience_details->description) ? $experience_details->description : ''),  
        );   
         
        $form['experience1']['eid'] = array(
            '#type' => 'hidden',
            '#value' => $eid,
        ); 

        $form['experience1']['buttonsField'] = array(
        '#type' => 'fieldset', 
        '#title' => t(''),
        '#prefix' => '<div id="buttonsDiv"  >',
        '#suffix' => '</div>',
        );
         
        $form['experience1']['buttonsField']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Submit'),   
            '#submit' => array('student_edit_experience_form_submit'),
            '#prefix' => '<div class="submit_clear_but">',
             
        );
        
        $form['experience1']['buttonsField']['cancel'] = array(
            '#type' => 'item', 
            '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/student/editexperience">Cancel</a></div>'),         
        );
          /*$form['experience1']['cancel'] = array(
            '#type' => 'button',
            '#value' => t('Cancel'),               
            '#suffix' => '</div>',
        ); */
    }
    $form['editbar'] = array(
        '#type' => 'item', 
        '#markup' => '
        <div class="wrapper">
        <ul class="menu">
        <li>
            <a class="active" id="nav_experience" href="' . $base_url . '/student/editexperience">
                <div class="experience borderRound" >
                <i rel="icon-briefcase" class="icon-briefcase" id="icon-briefcase"></i>
                </div>
                <div id="nav-lbl-experience">Experience</div>
            </a>
            
        </li>
        <li>
            <a id="nav_education" href="'.$base_url.'/student/editeducation">
            <div class="education borderRound" >
            <i rel="icon-education" class="icon-education" id="icon-education"></i></div><div id="nav-lbl-education">Education</div></a>
             
        </li>
        <li>
            <a id="nav_skills" href="'.$base_url.'/student/editskills">
            <div class="skills borderRound" >
            <i rel="icon-compass" class="icon-compass" id="icon-compass"></i></div><div id="nav-lbl-skill">Skills</div></a>
             
        </li>
        <li>
            <a id="nav_languages" href="'.$base_url.'/student/editlanguages">
            <div class="languages borderRound" >
            <i rel="icon-chat" class="icon-chat" id="icon-chat"></i></div><div id="nav-lbl-language">Languages</div></a>
            
        </li>
        <li>
            <a id="nav_add_info" href="'.$base_url.'/student/editadditionalinfo">
            <div class="additional_info borderRound" >
            <i rel="icon-additional" class="icon-additional" id="icon-additional"></i></div><div id="nav-lbl-additional">Additional Info</div></a>
             
        </li>
        <li>
            <a id="nav_about_me" href="'.$base_url.'/student/aboutme">
            <div class="about_me borderRound" >
            <i rel="icon-user" class="icon-user" id="icon-user"></i></div><div id="nav-lbl-about">About Me</div></a>
             
        </li>
        <li>
            <a id="previewResumeNav" href="'.$base_url.'/student/resumepreview">
            <div class="preview borderRound" >
            <i class="icon-resume" id="icon-resume"></i>
            </div><div id="nav-lbl-preview">Preview Resume</div></a>
        </li>
        </ul>
    </div>'
    );
    $form['#attributes'] = array('enctype' => 'multipart/form-data');
    
    return $form;  
}
 
function student_edit_experience_form_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);    
     $date1 = explode('/', $date_joined);
     $date_joined = $date1[2].'-'.$date1[0].'-'.$date1[1];
     
     
    if(!isset($specializationsp) || $specializationsp=='')
        $specializationsp=0;
    if(!isset($specializationc) || $specializationc=='')
        $specializationc=0;
    if(!isset($location_state) || $location_state=='')
        $location_state=0;
    
    $params = array(
            'company_name' => $company_name,
            'industry' => $industry,
	    'date_joined' => $date_joined,
            //'date_left' => $date_left,
            'job_title' => $job_title,
            'position_level' => $position,
            'specialization_p' => $specializationp,
            'specialization_sp' => $specializationsp,
            'specialization_c' => $specializationc,
            'country' => $location_country,
            'state' => $location_state,
            'salary' => $salary,
            //'present' => $present,
            'description' => $description['value'],
            'created_date' => date('Y-m-d H:i:s')
         );
    if($present){
        $date_left = '0000-00-00';
        $params['present'] = 1;
    } else {
        $date2 = explode('/', $date_left);
        $date_left = $date2[2].'-'.$date2[0].'-'.$date2[1];
        $present = 0;
        $params['present'] = 0;
        $params['date_left'] = $date_left;
    }
    //echo "<pre>";print_r($params);exit;
        db_update('student_experience')
        ->fields($params)
        ->condition('uid', $user->uid, '=')
        ->condition('id', $eid, '=')
         ->execute();drupal_goto('student/editexperience');  
         audit_log('update', 'Experience updated in manage resume.');
}

function student_edit_experience_form_submit_add($form, &$form_state) {
    global $user; 
  //  echo "<pre>";print_r($form_state['input']);exit;
    extract($form_state['input']);
    if($op=='Add') {
     $date1 = explode('/', $date_joined);
     $date_joined = $date1[2].'-'.$date1[0].'-'.$date1[1];
      
     
    if(!isset($specializationsp) || $specializationsp=='')
        $specializationsp=0;
    if(!isset($specializationc) || $specializationc=='')
        $specializationc=0;
    
    if(!isset($location_state)){
        $location_state=0;
    }
    
    $params = array(
            'company_name' => $company_name,
            'industry' => $industry,
	    'date_joined' => $date_joined,
           // 'date_left' => $date_left,
            'job_title' => $job_title,
            'position_level' => $position,
            'specialization_p' => $specializationp,
            'specialization_sp' => $specializationsp,
            'specialization_c' => $specializationc,
            'country' => $location_country,
            'state' => $location_state,
            'salary' => $salary,
            //'present' => $present,
            'description' => $description['value'],
            'uid' => $user->uid,
         );
    if($present){
        $date_left = '0000-00-00';
        $params['present'] = 1;
    } else {
        $date2 = explode('/', $date_left);
        $date_left = $date2[2].'-'.$date2[0].'-'.$date2[1];
        $present = 0;
        $params['date_left'] = $date_left;
        $params['present'] = 0;
    }
    //echo "<pre>";print_r($params);
      db_insert('student_experience')
        ->fields($params)
        ->execute();
      audit_log('insert', 'Experience added in manage resume.');
    } else {
        
        $arr = array('experience_level' => $level  );
        if($level==0){
            $arr['working_year'] = $year;
            $arr['working_month'] = $month;
        }
        db_update('student_details')
        ->fields($arr)
        ->condition('uid',$user->uid,'=')
        ->execute();        
    }
}



function editexperience($id) {       
    /*Decrypting the data using configration key*/  
    //$decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    //$id=(int)decrypt($id,$decrypting_key);     
    $student_experience = db_select('student_experience')
        ->fields('student_experience')    
        ->condition('id', $id, '=')
        ->execute()->fetchAll();
    return $student_experience[0];      
}

