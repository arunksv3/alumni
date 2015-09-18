<?php 
function student_update_resume_form($form, &$form_state) {
    global $user;
    global $base_url;
    resume_create_validation();
    $student_details = db_select('student_details')
        ->fields('student_details')
        ->condition('uid', $user->uid, '=')
        ->execute();
    $count = $student_details->rowCount(); 
     
    while ($row = $student_details->fetchAssoc($student_details)) {
        $modified_date =   date('m/d/Y H:i:s',strtotime($row['modified_date']));
        $resume = $row['resume'];
     }
 
    $form = array();
    
    $form['settings']= array(
         '#type' => 'fieldset',
          '#prefix' => '<div>',
        '#suffix' => '</div>',
    );
    
    $form['settings']['title']= array(
         '#type' => 'item',
        '#markup' => '<h2>Upload Resume</h2>'
           
    );
    
    if (file_exists($resume)) {
    $headers = file_download_headers($resume);
    if (count($headers)) {
      file_transfer($resume, $headers);
    }
   // drupal_access_denied();
  }
  
  if($resume!='') {
    $fileName = pathinfo($resume)['basename'];
    $form['settings']['filename']= array(
        '#type' => 'item',
        '#title' => t('File Name:'),
        '#markup' => '<div>'.$fileName.'&nbsp;&nbsp;&nbsp;<a href="'.file_create_url($resume).'">View</a> '
        . '| <a href="'.$base_url.'/student/deleteresume">Delete</a></div>',
    );
    
    $form['settings']['modified_date']= array(
        '#type' => 'item',
        '#title' => t('Uploaded:'),
        '#markup' => '<div>'.$modified_date.'</div>',
    );
  }
    
    $form['settings']['resume'] = array(
            '#id' => 'resume',
            '#type' => 'file',
            '#title' => t('Resume <span class="form-required">*</span>'),
            '#description' => 'Your file must be in Word (.doc or .docx), Text (.txt), Rich Text (.rtf) or PDF (.pdf) format'
        ); 
    $form['settings']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#prefix' => '<div class="submit_clear_but">',        
    );

    $form['settings']['cancel_aboutme'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/student/updateresume">Cancel</a>'),
        '#suffix' => '</div>',
    );

    $form['#attributes'] = array('enctype' => 'multipart/form-data');

    return $form;
}

function student_update_resume_form_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);  
    
    
    $resume_name = $_FILES["files"]["name"]['resume'];     
    // Move files to destination folder
    $resume_temp = $_FILES["files"]["tmp_name"]['resume'];
    $resume = time().'_'.$resume_name;
    $dest_resume = 'public://uploads/resumes/'.$resume;
    move_uploaded_file($resume_temp, $dest_resume);
    
        db_update('student_details')
        ->fields(array(
            'resume' => $dest_resume
          ))
        ->condition('uid', $user->uid, '=')
         ->execute(); 
    audit_log('update', 'account details updated.');
    drupal_set_message(t('Resume Updated'));
}

function student_update_resume_form_validate($form, &$form_state) {   
    
    // file extension validation
    $file_logo = $_FILES["files"]["name"]['resume'];
    $allowed =  array('doc','docx' ,'pdf');
    if($file_logo!=""){
        $logoext = pathinfo($file_logo, PATHINFO_EXTENSION);        
        if(!in_array($logoext, $allowed) ) {
            form_set_error('resume', t('Invalid file extension for resume'));
        }
    } else {
        form_set_error('resume',t('Upload Resume'));
     }
    
     
}