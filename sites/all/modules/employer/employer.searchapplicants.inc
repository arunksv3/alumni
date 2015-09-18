<?php

/**
 * Function for handling ajax request
 */
function employer_searchapplicants_state_list_ajax($form, $form_state) {
    // Return the dropdown list including the wrapper
    return $form['custom_keyword']['employer_details']['address']['wrapper'];
}

function employer_levelofqualifcation_ajax($form, $form_state) {
    // Return the dropdown list including the wrapper
    return $form['custom_keyword']['educational']['awards_child']['wrapper'];
}

function employer_levelofqualifcation_ajax_second($form, $form_state) {
    // Return the dropdown list including the wrapper
    return $form['custom_keyword']['educational']['awards_child_second']['wrapper'];
}

function employer_candidate_prefered_country_state_list_ajax($form, $form_state) {
    // Return the dropdown list including the wrapper
    return $form['custom_keyword']['candidate_prefered_address']['wrapper'];
}

function employer_searchapplicants_subparrent_list_ajax($form, $form_state) {
    // Return the dropdown list including the wrapper
    return $form['custom_keyword']['employer_details']['wrapper'];
}

function employer_searchapplicants_child_list_ajax($form, $form_state) {
    // Return the dropdown list including the wrapper
    return $form['custom_keyword']['employer_details']['wrapper_child'];
}

/* View applicants function */

function employer_searchapplicants($form, &$form_state) {
    global $base_url;
    drupal_add_js(drupal_get_path('module', 'employer') . '/js/employer_search.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') . '/css/jquery-ui.css');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery-ui.js');
    drupal_add_js('jQuery(document).ready(function($) { 
        $("#from").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
          $("#to").datepicker("option","minDate", selected)
        }
        });
        $("#to").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
         $("#from").datepicker("option","maxDate", selected)
        }
        });
        $("#graduation_date_from").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
          $("#graduation_date_to").datepicker("option","minDate", selected)
        }
        });
        $("#graduation_date_to").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
         $("#graduation_date_from").datepicker("option","maxDate", selected)
        }
        });
        $("#graduation_date_from_second").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
          $("#graduation_date_to_second").datepicker("option","minDate", selected)
        }
        });
        $("#graduation_date_to_second").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
         $("#graduation_date_from_second").datepicker("option","maxDate", selected)
        }
        });
        $("#graduation_date_from_third").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
          $("#graduation_date_to_third").datepicker("option","minDate", selected)
        }
        });
        $("#graduation_date_to_third").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
         $("#graduation_date_from_third").datepicker("option","maxDate", selected)
        }
        });        
        $(".popup").click(function () {                    
          window.open($(this).attr("href"), "PopupWindow", "width=600,height=600,scrollbars=yes,resizable=no");
          return false;
        });
        });', 'inline');
    drupal_add_js('function updateKeyword_search(val){ 
            if(val==0)
              document.getElementsByName("keyword")[0].placeholder = "Experience";            
            else if(val==1)
              document.getElementsByName("keyword")[0].placeholder = "Education & Key Skills";
            else
             document.getElementsByName("keyword")[0].placeholder = "Entire Resume";
             
        }', 'inline');
    $form = array();
    $form['custom_keyword'] = array(
        '#type' => 'fieldset',
    );
    $form['custom_keyword']['keyword'] = array(
        '#title' => 'Keyword - <span>A word or phrase to match relevant candidate data.</span>',
        '#type' => 'textfield',
        '#attributes' => array('placeholder' => t('Experience')),
    );

    $form['custom_keyword']['resume_title'] = array(
        '#type' => 'radios',
        '#title' => t('Search within'),
        '#options' => array(t('Experience'), t('Education & Key Skills'), t('Entire Resume')),
        '#attributes' => array('onclick' => "updateKeyword_search(this.value)"),
        '#default_value' => 2,
    );

    $form['custom_keyword']['experience_level'] = array(
        '#type' => 'radios',
        '#title' => t('Experience level'),
        '#options' => experience_level(),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );

    $form['custom_keyword']['employer_details'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="EmploymentDetails">',
        '#suffix' => '</div>',
    );

    $form['custom_keyword']['educational'] = array(
        '#type' => 'item',
    );
    $form['custom_keyword']['educational']['educational_details'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Educational Details <span> - Search for candidate employment details</span></h2>',
    );

    $form['custom_keyword']['educational']['levelofqualifcation'] = array(
        '#type' => 'select',
        '#title' => t('Highest Qualifcation:'),
        '#options' => levelofqualifcation(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'employer_levelofqualifcation_ajax',
            'wrapper' => 'levelofqualifcation-list-wrapper',
        ),
        '#prefix' => '<div class="col-md-5 inner-cont">'
    );
    // Wrapper for state dropdown list
    $form['custom_keyword']['educational']['awards_child']['wrapper'] = array(
        '#prefix' => '<div id="levelofqualifcation-list-wrapper">',
        '#suffix' => '</div>',
    );
    if ((isset($form_state['values']['levelofqualifcation']) && $form_state['values']['levelofqualifcation'] != "")) {
        $qid = $form_state['values']['levelofqualifcation'];
        $levelof_qualifcation_awards = db_select('levelof_qualifcation')
                ->fields('levelof_qualifcation')
                ->condition('qid', $qid, '=')
                ->execute()
                ->fetchAll();
        $levelof_qualifcation_awards_arr[''] = 'Select';
        foreach ($levelof_qualifcation_awards as $value):
            $levelof_qualifcation_awards_arr[$value->id] = $value->name;
        endforeach;
        if (count($levelof_qualifcation_awards_arr) > 0) {
            // Province dropdown list
            $form['custom_keyword']['educational']['awards_child']['wrapper']['awards'] = array(
                '#type' => 'select',
                '#options' => $levelof_qualifcation_awards_arr,
            );
        }
    }
    $form['custom_keyword']['educational']['fieldsofstudy'] = array(
        '#type' => 'select',
        '#title' => t('Fields of Study:'),
        '#options' => array(""=>"Select")+fields_of_study(),
        '#suffix' => '</div>'
    );

    $form['custom_keyword']['educational']['second_radio'] = array(
        '#type' => 'radios',
        '#options' => array('1' => 'and', '2' => 'or'),
        '#prefix' => '<div class="col-md-2 inner-cont">',
        '#suffix' => '</div>',
    );
    $form['custom_keyword']['educational']['levelofqualifcation_second'] = array(
        '#type' => 'select',
        '#title' => t('Second Highest Qualifcation:'),
        '#options' => levelofqualifcation(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'employer_levelofqualifcation_ajax_second',
            'wrapper' => 'levelofqualifcation-list-wrapper-second',
        ),
        '#prefix' => '<div class="col-md-5 inner-cont">'
    );
    // Wrapper for state dropdown list
    $form['custom_keyword']['educational']['awards_child_second']['wrapper'] = array(
        '#prefix' => '<div id="levelofqualifcation-list-wrapper-second">',
        '#suffix' => '</div>',
    );
    if ((isset($form_state['values']['levelofqualifcation_second']) && $form_state['values']['levelofqualifcation_second'] != "")) {
        $qid = $form_state['values']['levelofqualifcation_second'];
        $levelof_qualifcation_awards = db_select('levelof_qualifcation')
                ->fields('levelof_qualifcation')
                ->condition('qid', $qid, '=')
                ->execute()
                ->fetchAll();
        $levelof_qualifcation_awards_arr[''] = 'Select';
        foreach ($levelof_qualifcation_awards as $value):
            $levelof_qualifcation_awards_arr[$value->id] = $value->name;
        endforeach;
        if (count($levelof_qualifcation_awards_arr) > 0) {
            // Province dropdown list
            $form['custom_keyword']['educational']['awards_child_second']['wrapper']['awards_second'] = array(
                '#type' => 'select',
                '#options' => $levelof_qualifcation_awards_arr,
            );
        }
    }
    $form['custom_keyword']['educational']['fieldsofstudy_second'] = array(
        '#type' => 'select',
        '#title' => t('Fields of Study:'),
        '#options' => array(""=>"Select")+fields_of_study(),
        '#suffix' => '</div>'
    );

    $form['custom_keyword']['skill_set'] = array(
        '#type' => 'item',
        '#markup' => '<h2> Skill Set</h2>',
    );
    $form['custom_keyword']['skills'] = array(
        '#type' => 'textfield',
        '#id' => 'skills',
        '#title' => 'Skills',
        '#description' => '(Enter keywords with commas in between)',
    );
    $form['custom_keyword']['other_languages'] = array(
        '#type' => 'textfield',
        '#id' => 'other_languages',
        '#title' => 'Other Languages',
        '#description' => '(enter keywords with commas in between)',
    );
    $form['custom_keyword']['display_options'] = array(
        '#type' => 'item',
        '#markup' => '<h2> Display Options</h2>',
    );
    $form['custom_keyword']['exp_salary'] = array(
        '#type' => 'fieldset',
        '#title' => 'Candidate Expected Salary - <span>Enter Zero to pull candidates without salary</span>',
    );
    $form['custom_keyword']['exp_salary']['exp_salary_from'] = array(
        '#type' => 'textfield',
        '#id' => 'exp_salary_from',
        '#title' => 'From',
    );
    $form['custom_keyword']['exp_salary']['exp_salary_to'] = array(
        '#type' => 'textfield',
        '#id' => 'exp_salary_to',
        '#title' => 'To',
        '#description' => '(in MYR)',
    );
    $form['custom_keyword']['candidate_prefered_country'] = array(
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => array('' => 'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'employer_candidate_prefered_country_state_list_ajax',
            'wrapper' => 'candidate_prefered_country-list-wrapper',
        ),
        '#default_value' => (isset($country) ? $country : ''),
    );

    // Wrapper for state dropdown list
    $form['custom_keyword']['candidate_prefered_address']['wrapper'] = array(
        '#prefix' => '<div id="candidate_prefered_country-list-wrapper">',
        '#suffix' => '</div>',
    );

    if ((isset($form_state['values']['candidate_prefered_country']) && $form_state['values']['candidate_prefered_country'] != "")) {
        $cid = $form_state['values']['candidate_prefered_country'];
        $state_list = db_select('states_list')
                ->fields('states_list')
                ->condition('cid', $cid, '=')
                ->execute()
                ->fetchAll();
        $state_list_arr[0] = 'Select';
        foreach ($state_list as $value):
            $state_list_arr[$value->id] = $value->name;
        endforeach;
        if (count($state_list) > 0) {
            // Province dropdown list
            $form['custom_keyword']['candidate_prefered_address']['wrapper']['candidate_prefered_state'] = array(
                '#type' => 'select',
                '#title' => t('State'),
                '#multiple' => TRUE,
                '#options' => $state_list_arr,
            );
        }
    }
    $form['custom_keyword']['nationality'] = array(
        '#type' => 'select',
        '#title' => t('Nationality'),
        '#options' => array('' => 'Select') + alumni_std_states_list(),
    );


    $form['custom_keyword']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Search'),
        '#submit' => array('employer_filterjobs_submit'),
        '#prefix' => '<div class="search_but">'
    );

    $form['custom_keyword']['clear'] = array(
        '#type' => 'item',
        '#markup' => t('<a  class="search_clear" href="' . $base_url . '/employer/searchapplicants">Clear</a>'),
        '#suffix' => '</div>'
    );
    extract($form_state['input']);



    $sids = array();
    $sids1 = array();
    $sids2 = array();
    $sids3 = array();
    $sids4 = array();
    $sids5 = array();
    $kids = array();
    $eids = array();
    $skids = array();
    $dids = array();
    $flagkids = false;
    $flageids = false;
    $flagskids = false;
    $flagdids = false;
    if (isset($keyword) && $keyword != "") {
        $flagkids = true;
        if ($resume_title == 0) {
            $query5 = db_select('student_experience', 'se');
            $query5->join('specializations_p', 'p', 'p.sid=se.specialization_p');
            $query5->join('specializations_sp', 'sp', 'sp.sid=se.specialization_sp');
            $query5->join('specializations_c', 'sc', 'sc.sid=se.specialization_c');
            $query5->join('countries_list', 'c', 'c.id=se.country');
            $query5->leftjoin('states_list', 's', 's.id=se.state');
            $query5->fields('se', array('uid'));
            $db_or = db_or();
            $db_or->condition('se.job_title', '%' . $keyword . '%', 'like');
            $db_or->condition('p.name', '%' . $keyword . '%', 'like');
            $db_or->condition('sp.name', '%' . $keyword . '%', 'like');
            $db_or->condition('sc.name', '%' . $keyword . '%', 'like');
            $db_or->condition('c.name', '%' . $keyword . '%', 'like');
            $db_or->condition('s.name', '%' . $keyword . '%', 'like');
            $db_or->condition('se.salary', $keyword, '=');
            $query5->condition($db_or);
            $result = $query5->execute()->fetchAll();
            foreach ($result as $val) {
                $kids[] = $val->uid;
            }
        }
        if ($resume_title == 1) {
            $query6 = db_select('student_qualification', 'sq');
            $query6->join('qualifcation', 'q', 'sq.qualification=q.id');
            $query6->join('levelof_qualifcation', 'ql', 'sq.qualification_level=ql.id');
            $query6->join('fields_of_study', 'fs', 'sq.field_of_study=fs.id');
            $query6->join('student_skills', 's', 's.uid=sq.uid');
            $query6->join('student_languages', 'l', 'l.uid=sq.uid');
            $query6->fields('sq');
            $db_or = db_or();
            $db_or->condition('q.name', '%' . $keyword . '%', 'like');
            $db_or->condition('ql.name', '%' . $keyword . '%', 'like');
            $db_or->condition('fs.name', '%' . $keyword . '%', 'like');
            $db_or->condition('s.skills', '%' . $keyword . '%', 'like');
            $db_or->condition('l.language', '%' . $keyword . '%', 'like');
            $query6->condition($db_or);
            $result = $query6->execute()->fetchAll();
            foreach ($result as $val) {
                $kids[] = $val->uid;
            }
        }
        if ($resume_title == 2) {

            $query7 = db_select('student_experience', 'se');
            $query7->join('specializations_p', 'p', 'p.sid=se.specialization_p');
            $query7->join('specializations_sp', 'sp', 'sp.sid=se.specialization_sp');
            $query7->join('specializations_c', 'sc', 'sc.sid=se.specialization_c');
            $query7->join('countries_list', 'c', 'c.id=se.country');
            $query7->leftjoin('states_list', 's', 's.id=se.state');
            $query7->fields('se', array('uid'));
            $db_or = db_or();
            $db_or->condition('se.job_title', '%' . $keyword . '%', 'like');
            $db_or->condition('p.name', '%' . $keyword . '%', 'like');
            $db_or->condition('sp.name', '%' . $keyword . '%', 'like');
            $db_or->condition('sc.name', '%' . $keyword . '%', 'like');
            $db_or->condition('c.name', '%' . $keyword . '%', 'like');
            $db_or->condition('s.name', '%' . $keyword . '%', 'like');
            $db_or->condition('se.salary', $keyword, '=');
            $query7->condition($db_or);
            $result = $query7->execute()->fetchAll();
            foreach ($result as $val) {
                $sids1[] = $val->uid;
            }

            $query8 = db_select('student_qualification', 'sq');
            $query8->join('qualifcation', 'q', 'sq.qualification=q.id');
            $query8->join('levelof_qualifcation', 'ql', 'sq.qualification_level=ql.id');
            $query8->join('fields_of_study', 'fs', 'sq.field_of_study=fs.id');
            $query8->join('student_skills', 's', 's.uid=sq.uid');
            $query8->join('student_languages', 'l', 'l.uid=sq.uid');
            $query8->fields('sq');
            $db_or = db_or();
            $db_or->condition('q.name', '%' . $keyword . '%', 'like');
            $db_or->condition('ql.name', '%' . $keyword . '%', 'like');
            $db_or->condition('fs.name', '%' . $keyword . '%', 'like');
            $db_or->condition('s.skills', '%' . $keyword . '%', 'like');
            $db_or->condition('l.language', '%' . $keyword . '%', 'like');
            $query8->condition($db_or);
            $result = $query8->execute()->fetchAll();
            foreach ($result as $val) {
                $sids2[] = $val->uid;
            }


            $query9 = db_select('student_details', 'sd');
            $query9->join('countries_list', 'c', 'sd.country=c.id');
            $query9->join('states_list', 's', 'sd.state=s.id');
            $query9->fields('sd');
            $db_or = db_or();
            $db_or->condition('sd.expected_salary', $keyword, '=');
            $db_or->condition('c.name', '%' . $keyword . '%', 'like');
            $db_or->condition('s.name', '%' . $keyword . '%', 'like');
            $query9->condition($db_or);
            $result = $query9->execute()->fetchAll();

            foreach ($result as $val) {
                $sids3[] = $val->uid;
            }
            $kids = array_unique(array_merge($sids1, $sids2, $sids3));
        }
    }// else{
    if ((isset($levelofqualifcation) && $levelofqualifcation != "") || (isset($awards) && $awards != "") || (isset($fieldsofstudy) && $fieldsofstudy != "")) {
        $query10 = db_select('student_qualification')
                ->fields('student_qualification');
        if (isset($levelofqualifcation) && $levelofqualifcation != "") {
            $flageids = true;
            $query10->condition('qualification', $levelofqualifcation, '=');
        }
        if (isset($awards) && $awards != "") {
            $flageids = true;
            $query10->condition('qualification_level', $awards, '=');
        }
        if (isset($fieldsofstudy) && $fieldsofstudy != "") {
            $flageids = true;
            $query10->condition('field_of_study', $fieldsofstudy, '=');
        }
        $result = $query10->execute()->fetchAll();
        foreach ($result as $val) {
            $sids4[] = $val->uid;
        }
    }
    if ((isset($levelofqualifcation_second) && $levelofqualifcation_second != "") || (isset($awards_second) && $awards_second != "") || (isset($fieldsofstudy_second) && $fieldsofstudy_second != "")) {
        $query11 = db_select('student_qualification')
                ->fields('student_qualification');
        if (isset($levelofqualifcation_second) && $levelofqualifcation_second != "") {
            $flageids = true;
            $query11->condition('qualification', $levelofqualifcation_second, '=');
        }
        if (isset($awards_second) && $awards_second != "") {
            $flageids = true;
            $query11->condition('qualification_level', $awards_second, '=');
        }
        if (isset($fieldsofstudy_second) && $fieldsofstudy_second != "") {
            $flageids = true;
            $query11->condition('field_of_study', $fieldsofstudy_second, '=');
        }

        $result = $query11->execute()->fetchAll();
        foreach ($result as $val) {
            $sids5[] = $val->uid;
        }
    }
    if (isset($second_radio) && $second_radio == '1') {
        //$sids  = array_merge($sids,array_unique(array_intersect($sids1,$sids2)));
        $eids = array_unique(array_intersect($sids4, $sids5));
    } else {//if(isset($second_radio) && $second_radio=='2'){
        //$sids = array_merge($sids,array_unique(array_merge($sids1,$sids2)));
        $eids = array_unique(array_merge($sids4, $sids5));
    }


    if (isset($skills) && $skills != "") {
        $flagskids = true;
        $arrSkills = explode(',', $skills);
        foreach ($arrSkills as $val) {
            $query2 = db_select('student_skills')
                    ->fields('student_skills')
                    ->condition('skills', '%' . $val . '%', 'like');
            $result = $query2->execute()->fetchAll();
            foreach ($result as $val) {
                array_push($skids, $val->uid);
            }
        }
    }

    if (isset($other_languages) && $other_languages != "") {
        $flagskids = true;
        $arrLanguages = explode(',', $other_languages);
        foreach ($arrLanguages as $val) {
            $query3 = db_select('student_languages')
                    ->fields('student_languages')
                    ->condition('language', '%' . $val . '%', 'like');
            $result = $query3->execute()->fetchAll();
            foreach ($result as $val) {
                array_push($skids, $val->uid);
            }
        }
    }



    $query4 =  db_select('student_details','s');
    $query4->leftJoin('stdinfo', 'i', 's.uid = i.uid');
    $query4->fields('s');
    $query4->fields('i', array('ctzship','uid'));     

    if (isset($exp_salary_from) && $exp_salary_from > 0 && isset($exp_salary_to) && $exp_salary_to > 0) {
        $query4->condition('expected_salary', $exp_salary_from, '>=');
        $query4->condition('expected_salary', $exp_salary_to, '<=');
        $flagdids = true;
    }

    if (isset($candidate_prefered_country) && $candidate_prefered_country != "") {
        $query4->condition('country', $candidate_prefered_country, '=');
        $flagdids = true;
    }
    
    if (isset($nationality) && $nationality != "") { 
        $query4->condition('ctzship', $nationality, '=');
        $flagdids = true;
    }    

    if (isset($candidate_prefered_state) && $candidate_prefered_state[0] > 0) {
        $query4->condition('state', $candidate_prefered_state, 'in');
        $flagdids = true;
    }

    if (isset($experience_level) && $experience_level != "") {
        $query4->condition('experience_level', $experience_level, '=');
        $flagdids = true;
    }
    if ($flagdids == true) {
        $result = $query4->execute()->fetchAll();        
        foreach ($result as $val) {
            array_push($dids, $val->uid);
        }
    }
    
     
//        echo "<pre>";
//        print_r($dids);print_r($skids);print_r($eids);print_r($kids);
//        echo "</pre>";
    if ($flagdids == true && $flagskids == true && $flageids == true && $flagkids == true)
        array_push($sids, array_intersect($dids, $skids, $eids, $kids));
    
    else if ($flagdids == true && $flagskids == true && $flagkids == true)
        array_push($sids, array_intersect($dids, $skids, $kids));
    
    else if ($flagdids == true && $flageids == true && $flagkids == true)
        $sids = array_merge($sids, array_intersect($dids, $eids, $kids));
    
    else if ($flagskids == true && $flageids == true && $flagkids == true)
        array_push($sids, array_intersect($skids, $eids, $kids));

    else if ($flagdids == true && $flagskids == true)
        array_push($sids, array_intersect($dids, $skids));
    
    else if ($flagdids == true && $flageids == true)
        array_push($sids, array_intersect($dids, $eids));
    
    else if (sizeof($dids) > 0 && $flagkids == true)
        array_push($sids, array_intersect($dids, $kids));

    else if ($flagskids == true && $flageids == true)
        array_push($sids, array_intersect($skids, $eids));
    
    else if ($flagskids == true && $flagkids == true)
        array_push($sids, array_intersect($skids, $kids));

    else if ($flagkids == true && $flageids == true)
        array_push($sids, array_intersect($kids, $eids));

    else{
        $sids = array_merge($dids, $skids, $eids, $kids);
    }
    

    // }


    $flag = false;
    foreach (array_values($sids) as $n) {

        if ((int) $n > 0) {
            $flag = true;
        }
    }

    if ($flag == false)
        $sids[0] = 0;
    
    
    //print_r($sids);
    //$query = db_select('student_details', 's');
    //$query->leftjoin('student_experience', 'e', 's.uid = e.uid');
    $query = db_select('stdinfo','i');
    $query->leftJoin('student_details', 's', 's.uid = i.uid');
    $query->leftJoin('student_qualification', 'q', 's.uid=q.uid');
    $query->leftJoin('student_skills', 'k', 's.uid=k.uid');
    $query->leftJoin('student_languages', 'l', 's.uid=l.uid');
    $query->leftJoin('qualifcation', 'qualifcation', 'qualifcation.id=q.qualification');
    $query->leftJoin('levelof_qualifcation', 'levelof_qualifcation', 'levelof_qualifcation.id=q.qualification_level');
    
    $query->fields('s', array());
    $query->fields('i', array('name', 'alt_email','ctzship','uid'));
    //$query->fields('e', array('experience_level')); 
    //$query->fields('q', array('qualification','qualification_level')); 
    //$query->fields('k', array('GROUP_CONCAT(skills)')); 
    $query->addExpression("GROUP_CONCAT(DISTINCT k.skills)", 'skills');
    $query->addExpression("GROUP_CONCAT(DISTINCT l.language)", 'languages');
    $query->addExpression("GROUP_CONCAT(DISTINCT qualifcation.name,',\t', levelof_qualifcation.name,'<br>'  ORDER BY q.highest ASC)", 'qualification_name');
    //$query->addExpression("GROUP_CONCAT(DISTINCT levelof_qualifcation.name)", 'qualification_level');
    
    if (isset($keyword)) {
        $query->condition('s.uid', $sids, 'in');
    }
    
    $header = array(
        array('data' => t('User ID'), 'width' => '7%'),
        array('data' => t('Name'), 'width' => '16%'),
        // array('data' => t('Resume Updated on'), 'width' => '12%'),
        array('data' => t('Experience Level'), 'width' => '9%'),
        array('data' => t('Education'), 'width' => '13%'),
        // array('data' => t('Second Highest Education'), 'width' => '13%'),
        array('data' => t('Expected Salary'), 'width' => '6%'),
        array('data' => t('Skills'), 'width' => '8%'),
        array('data' => t('Languages'), 'width' => '8%'),
        array('data' => t('Status'), 'width' => '8%'),
        array('data' => t('Options'), 'width' => '25%'),
    );

    $table_sort = $query->extend('TableSort') // Add table sort extender.
            ->orderByHeader($header); // Add order by headers.
//echo "<pre>"; print_r($query);exit; //echo "</pre>";
    $pager = $table_sort->extend('PagerDefault')
            ->orderBy('id', 'DESC')
            ->groupBy('s.uid')
            ->limit(10); // 10 rows per page.
 /*echo "<pre>";
        print_r($result);
        exit;*/
        
    $result = $pager->execute();
    $rows = array();
    

    $form['table_records'] = array(
        '#type' => 'fieldset',
    );
    $experience_level = experience_level();
    //echo "<pre>"; print_r($result);
    foreach ($result as $res):
        $modified_date = date("m-d-Y", strtotime($res->modified_date));
        if ($res->expected_salary > 0)
            $expected_salary = $res->expected_salary . ' (MYR)';
        else
            $expected_salary = "";

        $qualifaction = $res->qualification_name;
        $rows[] = array($res->uid, $res->name, $experience_level[$res->experience_level], $qualifaction, $expected_salary, $res->skills, $res->languages, '', '<div class="options"><a href="latestresume/' . encrypt($res->uid, variable_get('IIUM_Encryption_Key', NULL)) . '" class="popup" alt="VIEW" title="VIEW">View CV</a><a href="sendresumerequest/' . encrypt($res->uid, variable_get('IIUM_Encryption_Key', NULL)) . '" class="popup"  title="SEND RESUME REQUEST" alt="Send Resume Request">Send Resume Request</a><a href="directshortlist/' . encrypt($res->uid, variable_get('IIUM_Encryption_Key', NULL)) . '" class="popup"  title="SHORTLIST" alt="Shortlist">Shortlist</a></div>');
    endforeach;
    $form['table_records']['table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => t('No Records Found!')
    );

    $form['pager'] = array('#markup' => theme('pager'));
    return $form;
}

function levelofqualifcation() {
    $qualifcation = db_select('qualifcation')
            ->fields('qualifcation')
            ->execute()
            ->fetchAll();
    $qualifcation_arr[''] = 'Select';
    foreach ($qualifcation as $value):
        $qualifcation_arr[$value->id] = $value->name;
    endforeach;
    return $qualifcation_arr;
}

function experience_level() {
    return array(
        '1' => 'Fresh Graduate',
        '2' => 'Student seeking internship /part time',
        '0' => 'Working',
    );
}

/**
 * Get highest qualifcation table based on qualifcation table
 */
function student_qualifcation($id) {
    $student_qualifcation = db_select('qualifcation')
                    ->fields('qualifcation', array('name'))
                    ->condition('id', $id, '=')
                    ->execute()->fetch();
    return $student_qualifcation;
}

/**
 * Get highest qualifcation level table based on levelof_qualifcation table
 */
function student_qualifcation_level($id) {
    $student_qualifcation_level = db_select('levelof_qualifcation')
                    ->fields('levelof_qualifcation', array('name'))
                    ->condition('id', $id, '=')
                    ->execute()->fetch();
    return $student_qualifcation_level;
}
