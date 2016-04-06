﻿using UnityEngine;
using UnityEngine.UI;
using System.Collections;


namespace UnityGame.Tanks
{
    public class TankShooting : MonoBehaviour, IFireable
    {
        public int m_PlayerNumber = 1;              // Used to identify the different players.
        public Rigidbody m_Shell;                   // Prefab of the shell.
        public Transform m_FireTransform;           // A child of the tank where the shells are spawned.
        public Slider m_AimSlider;                  // A child of the tank that displays the current launch force.
        public AudioSource m_ShootingAudio;         // Reference to the audio source used to play the shooting audio. NB: different to the movement audio source.
        public AudioClip m_ChargingClip;            // Audio that plays when each shot is charging up.
        public AudioClip m_FireClip;                // Audio that plays when each shot is fired.
        public float m_MinLaunchForce = 15f;        // The force given to the shell if the fire button is not held.
        public float m_MaxLaunchForce = 30f;        // The force given to the shell if the fire button is held for the max charge time.
        public float m_MaxChargeTime = 0.75f;       // How long the shell can charge for before it is fired at max force.


        private string m_FireButton;                // The input axis that is used for launching shells.
        private float m_CurrentLaunchForce;         // The force that will be given to the shell when the fire button is released.
        private float m_ChargeSpeed;                // How fast the launch force increases, based on the max charge time.
        private bool m_Fired;                       // Whether or not the shell has been launched with this button press.
        private bool m_FireWish;                    // Is the user intending to fire?

        private bool m_FireEvent;

        [SerializeField]
        private float m_FireCooldown = 5.0f;        // How long is the cooldown between fires?
        private bool m_IsFireOnCooldown;            // Is the tank currently on cooldown for firing?

        private float m_NoiseRadius = 100f;

        private void OnEnable()
        {
            // When the tank is turned on, reset the launch force and the UI
            m_CurrentLaunchForce = m_MinLaunchForce;
            m_AimSlider.value = m_MinLaunchForce;
            m_IsFireOnCooldown = false;
            m_FireWish = false;
        }


        private void Start ()
        {
            // The fire axis is based on the player number.
            m_FireButton = "Fire" + m_PlayerNumber;

            // The rate that the launch force charges up is the range of possible forces by the max charge time.
            m_ChargeSpeed = (m_MaxLaunchForce - m_MinLaunchForce) / m_MaxChargeTime;
        }


        private void Update ()
        {
            // The slider should have a default value of the minimum launch force.
            m_AimSlider.value = m_MinLaunchForce;

            // If a fire has been requested and we can fire...
            if (m_FireWish)
            {
                m_FireWish = false;

                if (CanFire())
                {
                    m_IsFireOnCooldown = true;
                    
                    Launch();
                    StartCoroutine(StartGunCooldown(m_FireCooldown));
                }
            }

            // Charge-type logic...

            // If the max force has been exceeded and the shell hasn't yet been launched...
            if (m_CurrentLaunchForce >= m_MaxLaunchForce && !m_Fired)
            {
                // ... use the max force and launch the shell.
                m_CurrentLaunchForce = m_MaxLaunchForce;
                Launch ();
            }
            // Otherwise, if the fire button has just started being pressed...
            else if (Input.GetButtonDown (m_FireButton))
            {
                // ... reset the fired flag and reset the launch force.
                m_Fired = false;
                m_CurrentLaunchForce = m_MinLaunchForce;

                // Change the clip to the charging clip and start it playing.
                m_ShootingAudio.clip = m_ChargingClip;
                m_ShootingAudio.Play ();
            }
            // Otherwise, if the fire button is being held and the shell hasn't been launched yet...
            else if (Input.GetButton (m_FireButton) && !m_Fired)
            {
                // Increment the launch force and update the slider.
                m_CurrentLaunchForce += m_ChargeSpeed * Time.deltaTime;

                m_AimSlider.value = m_CurrentLaunchForce;
            }
            // Otherwise, if the fire button is released and the shell hasn't been launched yet...
            else if (Input.GetButtonUp (m_FireButton) && !m_Fired)
            {
                // ... launch the shell.
                Launch ();
            }
        }

        private void FixedUpdate()
        {
            BroadcastEvents();
        }

        IEnumerator StartGunCooldown(float cooldownTimer)
        {
            yield return new WaitForSeconds(cooldownTimer);

            m_IsFireOnCooldown = false;
        }

        public bool CanFire()
        {
            return !m_IsFireOnCooldown;
        }

        public void Fire ()
        {
            m_FireWish = true;
        }

        public void Launch(bool maximumForce = false)
        {
            // Set the fired flag so only Fire is only called once.
            m_Fired = true;

            // Create an instance of the shell and store a reference to it's rigidbody.
            Rigidbody shellInstance =
                Instantiate(m_Shell, m_FireTransform.position, m_FireTransform.rotation) as Rigidbody;

            // Instantly up to max?
            if(maximumForce)
            {
                m_CurrentLaunchForce = m_MaxLaunchForce;
            }

            // Set the shell's velocity to the launch force in the fire position's forward direction.
            shellInstance.velocity = m_CurrentLaunchForce * m_FireTransform.forward;

            // Change the clip to the firing clip and play it.
            m_ShootingAudio.clip = m_FireClip;
            m_ShootingAudio.Play();

            // Reset the launch force.  This is a precaution in case of missing button events.
            m_CurrentLaunchForce = m_MinLaunchForce;

            m_FireEvent = true;
        }

        public void BroadcastEvents()
        {
            if(m_FireEvent)
            {
                m_FireEvent = false;

                var objectsInRadius = Physics.OverlapSphere(transform.position, m_NoiseRadius, ~(LayerMask.NameToLayer("Players")));

                for (int i = 0; i < objectsInRadius.Length; ++i)
                {
                    if (objectsInRadius[i] != null && objectsInRadius[i].gameObject != gameObject)
                    {
                        var percepts = objectsInRadius[i].GetComponent<TankPercepts>();
                        if (percepts != null)
                        {
                            percepts.lastKnownPosition = transform.position;
                        }
                    }
                }
            }
        }
    }
}