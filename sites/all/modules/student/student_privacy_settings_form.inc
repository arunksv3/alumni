<?php 
function student_privacy_settings_form($form, &$form_state) {
    global $user;

    resume_create_validation();
    $student_details = db_select('student_details')
        ->fields('student_details')
        ->condition('uid', $user->uid, '=')
        ->execute();
    $count = $student_details->rowCount(); 
     
    while ($row = $student_details->fetchAssoc($student_details)) {
        $searchable = $row['searchable'];
    }
 
    $form = array();
    $form['settings']= array(
         '#type' => 'fieldset',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );
    $form['settings']['searchable'] = array(
            '#id' => 'searchable',
            '#type' => 'radios',
            '#title' => t('<h2>Privacy Setting</h2>'), 
            '#options' => array(
                1 => t('Searchable with Contact Details <span class="note_privacy">Allow employers to search for my resume and see my name and contact details.</span>'), 
                0 => t('Not Searchable <span class="note_privacy">Do not allow employers to search for my resume.</span>')
                ),
            //'#required' => TRUE,
            '#default_value' => (isset($searchable) ? $searchable : ''),
            
        );
    $form['settings']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#prefix' => '<div class="privacy_buttons">',
         
    );
    $form['settings']['cancel_aboutme'] = array(
        '#id' => 'Cancel',
        '#type' => 'button',
        '#value' => t('Cancel'),         
        '#suffix' => '</div>',
    );

     
     
    $form['#attributes'] = array('enctype' => 'multipart/form-data');

    return $form;
}

function student_privacy_settings_form_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);  
    
        db_update('student_details')
        ->fields(array(
            'searchable' => $searchable 
          ))
        ->condition('uid', $user->uid, '=')
         ->execute(); 
        drupal_set_message(t('Privacy Settings Updated'));
}