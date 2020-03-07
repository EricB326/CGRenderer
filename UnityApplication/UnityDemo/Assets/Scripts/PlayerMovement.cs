using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
    [SerializeField] private CharacterController m_controller;

    [SerializeField] private float m_playerSpeed = 12f;
    [SerializeField] private float m_gravity = -9.81f;
    [SerializeField] private float m_jumpheight = 3f;

    [SerializeField] private Transform m_groundCheck;
    [SerializeField] private float m_groundDistance = 0.4f;
    [SerializeField] LayerMask m_groundMask;

    [SerializeField] private ParticleSystem m_landingParticle;

    private Vector3 m_startingPos;
    private Vector3 m_velocity;
    private bool m_isGrounded;
    private bool m_previousGroundCheck;

    private Animator m_animator;

    private void Start()
    {
        m_animator = GetComponent<Animator>();
        m_startingPos = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        // Check if the invisible sphere has collided with the ground mask to know if object is on the floor
        m_isGrounded = Physics.CheckSphere(m_groundCheck.position, m_groundDistance, m_groundMask);
        if (!m_previousGroundCheck && m_isGrounded)
        {
            m_landingParticle.Emit(20);
            m_animator.SetBool("hasJumped", false);
        }
        m_previousGroundCheck = m_isGrounded;

        if (m_isGrounded && m_velocity.y < 0.0f)
        {
            // Make sure the object is completely planted on the ground
            m_velocity.y = -1.0f;
        }

        // Only allow forwards/backward movement
        float z = Input.GetAxis("Vertical");
        Vector3 move = transform.forward * z;

        // Check if the animation playing should be the running anim
        if (move != Vector3.zero)
            m_animator.SetBool("isMoving", true);
        else
            m_animator.SetBool("isMoving", false);

        // Move the character
        m_controller.Move(move * m_playerSpeed * Time.deltaTime);

        if (Input.GetButtonDown("Jump") && m_isGrounded)
        {
            m_velocity.y = Mathf.Sqrt(m_jumpheight * -2f * m_gravity);
            m_animator.SetBool("hasJumped", true);
        }

        // Calculate player gravity
        m_velocity.y += m_gravity * Time.deltaTime;

        // Multipled by time again because gravity equals:
        // delta Y = 1/2gravity * time^2.
        m_controller.Move(m_velocity * Time.deltaTime);

        if (transform.position.y <= -15f)
        {
            transform.position += new Vector3(0.0f, 0.0f, -0.85f);
        }
    }

    public void ResetPlayerPos()
    {
        transform.position = m_startingPos;
    }
}
