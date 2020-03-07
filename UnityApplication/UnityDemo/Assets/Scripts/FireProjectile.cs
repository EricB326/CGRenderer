using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireProjectile : MonoBehaviour
{
    [SerializeField] private Rigidbody m_projectileToSpawn;
    [SerializeField] private Transform m_spawnPosition;

    [SerializeField] private float m_projectileSpeed = 10000f;

    public void LaunchProjectile()
    {
        Rigidbody rb = Instantiate(m_projectileToSpawn, m_spawnPosition.position, m_spawnPosition.rotation);
        rb.velocity = transform.forward * m_projectileSpeed * Time.deltaTime;
    }
}
